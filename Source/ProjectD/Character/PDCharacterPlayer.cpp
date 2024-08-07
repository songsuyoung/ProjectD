// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDCharacterPlayer.h"
#include "Player/PDPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PDInputComponent.h"
#include "Weapons/PDWeapon.h"
#include "UI/PDHPWidgetComponent.h"
#include "UI/PDStat.h"
#include "Animation/AnimMontage.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Character/PDSkillComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Net/UnrealNetwork.h"

#include "GameData/PDDataManager.h"
#include "Interfaces/PDPostMissionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "ProjectD.h"

APDCharacterPlayer::APDCharacterPlayer() : bCanAttack(false)
{

	//Default
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	//Default AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));

	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimClass(AnimInstanceRef.Class);
	}

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bUsePawnControlRotation = false; //QuaterView�� ȸ������ �ʴ´�.
	CameraSpringArm->TargetArmLength = 500.f;
	CameraSpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	PDInputComponent = CreateDefaultSubobject<UPDInputComponent>(TEXT("InputComponent"));	

	HPWidget = CreateDefaultSubobject<UPDHPWidgetComponent>(TEXT("HPWidgetComponent"));
	/*�Ӹ��� ������ ������ ���� ����ٴϸ鼭 ����������.*/
	HPWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("head"));
	HPWidget->SetWidgetSpace(EWidgetSpace::Screen); //��ũ���� ��� ����
	HPWidget->SetDrawSize(FVector2D(30.f, 60.f));
	HPWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	/*HPWidget UserWidget*/
	static ConstructorHelpers::FClassFinder<UPDStat> StatWidget(TEXT("/Game/ProjectD/HUD/UI/WB_Stat.WB_Stat_C"));

	if (StatWidget.Class)
	{
		HPWidget->SetWidgetClass(StatWidget.Class);
	}

	AttackComponent = CreateDefaultSubobject<UPDSkillComponent>(TEXT("SkillComponent"));
	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
}

USpringArmComponent* APDCharacterPlayer::GetSpringArm()
{
	return CameraSpringArm;
}

int APDCharacterPlayer::SpringArmLength()
{
	if (CameraSpringArm == nullptr) return 0;
	return CameraSpringArm->TargetArmLength;
}

void APDCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PDInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
}

void APDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PC = Cast<APDPlayerController>(NewController);
}

void APDCharacterPlayer::OnRep_Controller()
{
	Super::OnRep_Controller();

	PC = GetController<APDPlayerController>();
}

void APDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (PC)
	{
		EnableInput(PC);
	}

	PDInputComponent->SetCharacterControl();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	Weapon = GetWorld()->SpawnActor<APDWeapon>(DefaultWeaponClass, SpawnParams);
	if (Weapon)
	{
		Weapon->Equip(this);
	}

	//���������� �����ϱ���... => 
	// NavMeshBoundsVolume NetLoadOnClinet�� ����������, Ŭ���̾�Ʈ�� �����ϸ鼭 �������� �� Nav ũ�⸦ ���� ���ؼ� �߻��ϴ� ��������
	// Runtime Generate �� true�� �������� ��, Ŭ���̾�Ʈ �� ����� �׺���̼� �ڽ� ũ�⸦ �������ؼ� �׺���̼� ���� �Լ� ����� ��������
	if (IsLocallyControlled())
	{
		SetPath();
	}
}


void APDCharacterPlayer::Skill(PDESkillType SkillType)
{
	PDEWeaponType WeaponType = Weapon->GetWeaponType();

	TArray<FAttackSkillDelegateWrapper> FoundDelegates;
	AttackComponent->AttackDelegate.MultiFind(WeaponType, FoundDelegates);

	for (const FAttackSkillDelegateWrapper& DelegateWrapper : FoundDelegates)
	{
		if (DelegateWrapper.AttackSkillDelegate.Find(SkillType) == false)
		{
			continue;
		}
		DelegateWrapper.AttackSkillDelegate[SkillType].Execute();
	}

}

void APDCharacterPlayer::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	//Volume�� ��ġ�� �����´�.
	//�̵��� ������ Update�� �����Ѵ�.
	//Ŭ���̾�Ʈ �󿡼��� ȣ��ȴ�.
	SetPath();
}

void APDCharacterPlayer::SetPath()
{
	Path->ClearSplinePoints(true); //������ �׷ȴ� ��� �����͸� �����.
	InitPath();

	UPDDataManager* DataManager = GetWorld()->GetSubsystem<UPDDataManager>();

	if (DataManager)
	{
		//�÷��̾�� ���� �̼ǿ� ���� ������ ������ �־����.
		//Type�� ���� ����.
		IPDPostMissionInterface* Mission = Cast<IPDPostMissionInterface>(DataManager->GetMission(PDEMissionType::Mission1));
		if (Mission)
		{
			FVector StartPos = GetActorLocation();
			FVector EndPos = Mission->GetPosition();
			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPos, EndPos);
		
			if (NavPath)
			{
				int Index = 0;
				for (auto& PathPoint : NavPath->PathPoints)
				{
					Path->AddSplinePointAtIndex(PathPoint, Index++, ESplineCoordinateSpace::World);
				}

				DrawPath(NavPath->PathPoints.Num());
			}
		}
	}
}

void APDCharacterPlayer::InitPath()
{
	for (int32 Index = 0; Index < PathMeshComp.Num(); Index++)
	{
		if (PathMeshComp[Index])
		{
			PathMeshComp[Index]->DestroyComponent();
		}
	}
}

void APDCharacterPlayer::DrawPath(int PathLen)
{
	
	for (int32 Index = 0; Index < PathLen - 1; Index++)
	{
		USplineMeshComponent* MeshComp = NewObject<USplineMeshComponent>(this, PathMeshClass);
	
		MeshComp->SetMobility(EComponentMobility::Stationary);
		MeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		MeshComp->RegisterComponentWithWorld(GetWorld());

		MeshComp->AttachToComponent(Path, FAttachmentTransformRules::KeepRelativeTransform);
		MeshComp->SetStartScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(1.f)));
		MeshComp->SetEndScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(1.f)));

		FVector StartPoint = Path->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local);
		FVector StartTangent = Path->GetTangentAtSplinePoint(Index, ESplineCoordinateSpace::Local);
		FVector EndPoint = Path->GetLocationAtSplinePoint(Index + 1, ESplineCoordinateSpace::Local);
		FVector EndTangent = Path->GetTangentAtSplinePoint(Index + 1, ESplineCoordinateSpace::Local);

		MeshComp->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PathMeshComp.Add(MeshComp);
	}
}

void APDCharacterPlayer::ClearPath()
{
	if (Path)
	{
		Path->ClearSplinePoints(true);
	}
}

void APDCharacterPlayer::ComboBegin()
{
	//�ִϸ��̼��� �����Ѵ�.
	bCanAttack = false;
	AttackComponent->ComboAttackIndex = 1;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayAttackAnimation();
}

void APDCharacterPlayer::ComboNext()
{
	PD_LOG(PDLog, Log, TEXT("ComboNext"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		PD_LOG(PDLog, Log, TEXT("ComboNext 2"));

		FName NextSection = AttackComponent->GetNextSection(Weapon->GetMaxComboLen());
		AnimInstance->Montage_JumpToSection(NextSection, Weapon->GetAnimMontage());

	}
}

void APDCharacterPlayer::ComboEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APDCharacterPlayer::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(Weapon->GetAnimMontage(), Weapon->GetPlayRate());
	}
}

void APDCharacterPlayer::ComboInit()
{
	PD_LOG(PDLog, Log, TEXT("ComboInit"));
	AttackComponent->ComboAttackIndex = 0;

}


void APDCharacterPlayer::OnRep_CanAttack()
{
	if (!bCanAttack)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void APDCharacterPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APDCharacterPlayer, bCanAttack);
}

void APDCharacterPlayer::ClientRPCBaseSkill_Implementation(APDCharacterPlayer* OtherPlayer)
{
	if (OtherPlayer)
	{
		OtherPlayer->PlayAttackAnimation();
	}
}