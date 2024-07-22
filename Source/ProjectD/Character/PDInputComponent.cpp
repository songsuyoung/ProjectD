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

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	MouseSensitivity = 4.f; //CDO 내에서는 4.0으로 지정
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

	FVector Destination = (CachedDestination - Owner->GetActorLocation()).GetSafeNormal();
	Owner->AddMovementInput(Destination);
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

	if (SpringArm)
	{
		float Pitch = LookAxisVector.Y * MouseSensitivity;
		float Yaw = LookAxisVector.X * MouseSensitivity; //감도 설정

		SpringArm->AddWorldRotation(FRotator(0.f, Yaw, 0.f)); // Yaw의 기준은 월드 상에서 동작
		SpringArm->AddLocalRotation(FRotator(Pitch, 0.f, 0.f)); //위 아래는 로컬 상에서 동작
	}
}

void UPDInputComponent::Zoom(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() > 0.f)
	{
		//원상태로 복귀
		CameraPos = FMath::Clamp(++CameraPos, MinCameraPosThreshold, MaxCameraPos);
	}
	else
	{
		//하나씩 증가
		CameraPos = FMath::Clamp(--CameraPos, MinCameraPosThreshold, MaxCameraPos);
	}
}
