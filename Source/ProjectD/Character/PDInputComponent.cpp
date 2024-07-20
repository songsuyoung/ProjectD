// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDInputComponent.h"
#include "Character/PDCharacterPlayer.h"
#include "Player/PDPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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
}

void UPDInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Owner = Cast<APDCharacterPlayer>(this->GetOwner());

	if (Owner)
	{
		PC = Owner->GetController<APDPlayerController>(); //입력이 들어올 때 초기화해준다. 
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPDInputComponent::Move);

	}
}

void UPDInputComponent::SetCharacterControl()
{
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

void UPDInputComponent::Move(const FInputActionValue& Value)
{
}
