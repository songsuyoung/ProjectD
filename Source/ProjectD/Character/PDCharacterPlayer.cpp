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

APDCharacterPlayer::APDCharacterPlayer()
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
	HPWidget->SetDrawSize(FVector2D(200.f, 40.f));
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

void APDCharacterPlayer::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return;
	}

	bIsAttacking = true;

	if (!AnimInstance->Montage_IsPlaying(Weapon->GetAnimMontage()))
	{
		/* 첫 콤보 시작시, ComboAttackIndex는 1을 나타낸다, 사실 1뿐만 아니라, 발생했냐 안했냐의 유무를 나타낸다.*/
		/* Notify가 끝나게 되면 0으로 자동으로 줄이기 때문에 콤보공격을 표현할 수 있다.*/
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->Montage_Play(Weapon->GetAnimMontage(), Weapon->GetPlayRate());
	}
	else
	{
		bIsFinalComboAttack = true;
		AttackComponent->ComboAttackIndex = 1;
	}
}

void APDCharacterPlayer::PlayMontageNotifyBegin()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return;
	}

	AttackComponent->ComboAttackIndex--; /*줄어드는데, 0이하가 아니라면, 콤보 애니메이션을 멈추지않는다.*/

	if (AttackComponent->ComboAttackIndex < 0)
	{
		/* 모든 애니메이션 */
		bIsAttacking = false;
		AttackComponent->ComboAttackIndex = 0;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		AnimInstance->StopAllMontages(0.35f); 
		/*0이 됐을 때엔 모든 콤보 공격이 끝났음을 의미해서, 애니메이션 몽타주를 멈춘다.*/
	}

	bIsFinalComboAttack = false;
}
/* 데디케이티드 환경에서는 애니메이션 노티파이가 진행되지 않는다. 처음엔 몰랐으나, 서버에서 작동안하는 걸 보고
알게 됐다. 이를 해결하기 위해서 ServerRPC를 사용해 모든 작업을 수행해주어야한다. */
void APDCharacterPlayer::ServerRPCEndedAttack_Implementation()
{
	PlayMontageNotifyBegin();
}
