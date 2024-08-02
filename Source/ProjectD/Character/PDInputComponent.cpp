// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDInputComponent.h"
#include "Character/PDCharacterPlayer.h"
#include "Player/PDPlayerController.h"
#include "Physics/PDCollision.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameData/PDESkillType.h"
#include "GameData/PDESkillType.h"

#include "ProjectD.h"

UPDInputComponent::UPDInputComponent()
{
	//InputContext Mapping
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContextMappingRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ProjectD/Inputs/IMC_Quater.IMC_Quater'"));

	if (InputContextMappingRef.Object)
	{
		InputMappingContext = InputContextMappingRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_Move.IA_Move'"));

	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_Look.IA_Look'"));

	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_Zoom.IA_Zoom'"));

	if (ZoomActionRef.Object)
	{
		ZoomAction = ZoomActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackBaseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_AttackBase.IA_AttackBase'"));

	if (AttackBaseActionRef.Object)
	{
		AttackBaseAction = AttackBaseActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackSkillQActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_AttackQ.IA_AttackQ'"));

	if (AttackSkillQActionRef.Object)
	{
		AttackSkillQAction = AttackSkillQActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackSkillWActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_AttackW.IA_AttackW'"));

	if (AttackSkillWActionRef.Object)
	{
		AttackSkillWAction = AttackSkillWActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackSkillEActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectD/Inputs/InputAction/IA_AttackE.IA_AttackE'"));

	if (AttackSkillEActionRef.Object)
	{
		AttackSkillEAction = AttackSkillEActionRef.Object;
	}

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	MouseSensitivity = 4.f; //CDO 내에서는 4.0으로 지정
	MouseWheelSensitivity = 20.f; //COD 에서 20.f으로 지정

	//1Level 기본 Weapon : 도끼
	// 
	//2Level 선택 
}

void UPDInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (Owner)
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &UPDInputComponent::OnMoveInputStarted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPDInputComponent::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &UPDInputComponent::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &UPDInputComponent::OnSetDestinationReleased);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UPDInputComponent::Look);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &UPDInputComponent::Zoom);

		EnhancedInputComponent->BindAction(AttackBaseAction, ETriggerEvent::Started, this, &UPDInputComponent::Attack, PDESkillType::Base);
		EnhancedInputComponent->BindAction(AttackSkillQAction, ETriggerEvent::Started, this, &UPDInputComponent::Attack, PDESkillType::SkillQ);
		EnhancedInputComponent->BindAction(AttackSkillWAction, ETriggerEvent::Started, this, &UPDInputComponent::Attack, PDESkillType::SkillW);
		EnhancedInputComponent->BindAction(AttackSkillEAction, ETriggerEvent::Started, this, &UPDInputComponent::Attack, PDESkillType::SkillE);
	}
}

void UPDInputComponent::SetCharacterControl()
{
	Owner = Cast<APDCharacterPlayer>(this->GetOwner());
	PC = Owner->GetController<APDPlayerController>();
	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->ClearAllMappings(); //모든 매핑 취소
			UInputMappingContext* NewMappingContext = InputMappingContext;

			if (NewMappingContext)
			{
				SubSystem->AddMappingContext(NewMappingContext, 0);
			}
		}
	}

	if (Owner)
	{
		CameraPos = Owner->SpringArmLength();
		MaxCameraPos = Owner->SpringArmLength();
	}
}

void UPDInputComponent::OnMoveInputStarted()
{
	if (PC == nullptr) return;
	
	PC->StopMovement(); //시작할 때 기존 움직임을 멈추고,
}

void UPDInputComponent::OnSetDestinationTriggered()
{
	if (PC == nullptr || Owner == nullptr) return;

	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult HitResult;
	bool bHitSuccessful = PC->GetHitResultUnderCursor(CCHANNEL_PDGROUND, true, HitResult);
	
	if (bHitSuccessful)
	{
		CachedDestination = HitResult.Location;
	}

	//클릭한 곳으로 회전하도록 한다.
	FVector Direction = CachedDestination - Owner->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::MakeRotFromX(Direction);
	LookAtRotation.Pitch = 0.f;
	LookAtRotation.Roll = 0.f;
	Owner->SetActorRotation(LookAtRotation); //바라보는 방향으로 회전
	
	FVector Destination = (Direction).GetSafeNormal();
	Owner->AddMovementInput(Destination); //이동
}

void UPDInputComponent::OnSetDestinationReleased()
{
	if (PC == nullptr) return;

	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC, CachedDestination);
	}

	FollowTime = 0.f;
}

void UPDInputComponent::Look(const FInputActionValue& Value)
{
	if (Owner == nullptr) return;

	//Yaw값과 Pitch값을 가져옴
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	//Spring Arm의 World좌표를 가져와야함.
	USpringArmComponent* SpringArm = Owner->GetSpringArm();
	if (SpringArm == nullptr)
	{
		PD_SUBLOG(PDLog, Error, TEXT("SpringArm is Not founded"));
		return;
	}

	float Pitch = LookAxisVector.Y * MouseSensitivity;
	float Yaw = LookAxisVector.X * MouseSensitivity; //감도 설정

	SpringArm->AddWorldRotation(FRotator(0.f, Yaw, 0.f)); // Yaw의 기준은 월드 상에서 동작
	SpringArm->AddLocalRotation(FRotator(Pitch, 0.f, 0.f)); //위 아래는 로컬 상에서 동작

}

void UPDInputComponent::Zoom(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	USpringArmComponent* SpringArm = Owner->GetSpringArm();

	if (SpringArm == nullptr)
	{
		PD_SUBLOG(PDLog, Error, TEXT("SpringArm is Not founded"));
		return;
	}
	//Direction 음수인지, 양수인지에 따라서 값이 결정되기 때문에,
	//덧셈과 곱셈을 사용해서 Pos값을 변하게 한다.
	CameraPos += Direction * MouseWheelSensitivity;
	CameraPos = FMath::Clamp(CameraPos, MinCameraPosThreshold, MaxCameraPos);
	SpringArm->TargetArmLength = CameraPos;
}

void UPDInputComponent::Attack(PDESkillType SkillType)
{
	//현재 공격중이면 공격하면 안된다.
	
	//입력받은 키를 전달할 예정
	Owner->Skill(SkillType);
}
