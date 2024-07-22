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
	MouseSensitivity = 4.f; //CDO �������� 4.0���� ����
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
			SubSystem->ClearAllMappings(); //��� ���� ���
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

	PC->StopMovement(); //������ �� ���� �������� ���߰�,
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

	//Yaw���� Pitch���� ������
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	//Spring Arm�� World��ǥ�� �����;���.
	USpringArmComponent* SpringArm = Owner->GetSpringArm();

	if (SpringArm)
	{
		float Pitch = LookAxisVector.Y * MouseSensitivity;
		float Yaw = LookAxisVector.X * MouseSensitivity; //���� ����

		SpringArm->AddWorldRotation(FRotator(0.f, Yaw, 0.f)); // Yaw�� ������ ���� �󿡼� ����
		SpringArm->AddLocalRotation(FRotator(Pitch, 0.f, 0.f)); //�� �Ʒ��� ���� �󿡼� ����
	}
}

void UPDInputComponent::Zoom(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() > 0.f)
	{
		//�����·� ����
		CameraPos = FMath::Clamp(++CameraPos, MinCameraPosThreshold, MaxCameraPos);
	}
	else
	{
		//�ϳ��� ����
		CameraPos = FMath::Clamp(--CameraPos, MinCameraPosThreshold, MaxCameraPos);
	}
}
