// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PDNextComboNotify.h"
#include "Character/PDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPDNextComboNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Pawn = Cast<APDCharacterPlayer>(MeshComp->GetOwner());

	if (Pawn)
	{
		Pawn->PlayMontageNotifyBegin();
	}
}

void UPDNextComboNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Pawn)
	{
		Pawn->PlayMontageNotifyBegin();
		Pawn->ServerRPCEndedAttack();
	}
}
