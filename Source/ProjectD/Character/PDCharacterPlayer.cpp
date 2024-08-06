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

#include "GameData/PDDataManager.h"
#include "Interfaces/PDPostMissionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "ProjectD.h"

APDCharacterPlayer::APDCharacterPlayer() : bCanAttackNextCombo(false)
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
	CameraSpringArm->bUsePawnControlRotation = false; //QuaterView는 회전하지 않는다.
	CameraSpringArm->TargetArmLength = 500.f;
	CameraSpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	PDInputComponent = CreateDefaultSubobject<UPDInputComponent>(TEXT("InputComponent"));	

	HPWidget = CreateDefaultSubobject<UPDHPWidgetComponent>(TEXT("HPWidgetComponent"));
	/*머리를 움직일 때마다 같이 따라다니면서 움직여야함.*/
	HPWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("head"));
	HPWidget->SetWidgetSpace(EWidgetSpace::Screen); //스크린상에 띄울 예정
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

	//서버에서만 동작하구나... => 
	// NavMeshBoundsVolume NetLoadOnClinet가 켜져있지만, 클라이언트에 복사하면서 생성됐을 때 Nav 크기를 잡지 못해서 발생하는 문제같음
	// Runtime Generate 를 true로 변경했을 때, 클라이언트 상 복사시 네비게이션 박스 크기를 재조정해서 네비게이션 관련 함수 사용이 가능해짐
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

void APDCharacterPlayer::ComboActionBegin()
{
	AttackComponent->ComboAttackIndex = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); //이동 못하도록함

	const float AnimPlayRate = Weapon->GetPlayRate();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Weapon->GetAnimMontage(), Weapon->GetPlayRate());
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APDCharacterPlayer::ComboEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Weapon->GetAnimMontage());
}

void APDCharacterPlayer::ComboCheck()
{
	if (bCanAttack == false) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance)
	{
		FName NextSection = AttackComponent->GetNextSection(Weapon->GetMaxComboLen());
		AnimInstance->Montage_JumpToSection(NextSection, Weapon->GetAnimMontage());
		PD_LOG(PDLog, Log, TEXT("6"));
		bCanAttack = false;
	}
}

void APDCharacterPlayer::ComboStart()
{
	if (AttackComponent->ComboAttackIndex == 0 || bCanAttack == false)
	{
		ServerRPCAttackAnimation();	
	}
}

void APDCharacterPlayer::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); //이동 못하도록함
	bCanAttackNextCombo = false;
	bCanAttack = false;
	AttackComponent->ComboAttackIndex = 0;
}

void APDCharacterPlayer::ServerRPCAttackAnimation_Implementation()
{
	MulticastRPCAttackAnimation();
}

void APDCharacterPlayer::MulticastRPCAttackAnimation_Implementation()
{
	if (AttackComponent->ComboAttackIndex == 0)
	{
		ComboActionBegin();
		return;
	}

	/*애니메이션 Begin/End에 의해서 설정 bCanAttackNextCombo*/
	if (bCanAttackNextCombo)
	{
		bCanAttack = true; /*다음 공격이 가능한가*/
	}
}

void APDCharacterPlayer::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	//Volume의 위치를 가져온다.
	//이동할 때마다 Update를 수행한다.
	//클라이언트 상에서만 호출된다.
	SetPath();
}

void APDCharacterPlayer::SetPath()
{
	Path->ClearSplinePoints(true); //이전에 그렸던 모든 포인터를 지운다.
	InitPath();

	UPDDataManager* DataManager = GetWorld()->GetSubsystem<UPDDataManager>();

	if (DataManager)
	{
		//플레이어는 현재 미션에 대한 정보를 가지고 있어야함.
		//Type은 변경 예정.
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