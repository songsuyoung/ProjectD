// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDInputComponent.h"
#include "Character/PDCharacterPlayer.h"
#include "Player/PDPlayerController.h"
#include "Physics/PDCollision.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void UPDInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (Owner)
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &UPDInputComponent::OnInputStarted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPDInputComponent::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &UPDInputComponent::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &UPDInputComponent::OnSetDestinationReleased);

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
}

void UPDInputComponent::OnInputStarted()
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
