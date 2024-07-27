// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PDComboAnimNotifyState.h"
#include "Character/PDCharacterPlayer.h"
void UPDComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Pawn = Cast<APDCharacterPlayer>(MeshComp->GetOwner());

	if (Pawn)
	{
		Pawn->IncreaseComboIdx();
	}
}

void UPDComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (Pawn)
	{
		Pawn->DecreaseComboIdx();
	}
}
