// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PDComboAnimNotifyState.h"
#include "Character/PDCharacterPlayer.h"
#include "ProjectD.h"
void UPDComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Pawn = Cast<APDCharacterPlayer>(MeshComp->GetOwner());

	if (Pawn)
	{
		Pawn->NextComboAvailable(true);
	}

}

void UPDComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (Pawn)
	{
		/*Notify가 끝난 이후에 들어오는 공격은 일반 공격으로 체크한다.*/
		Pawn->NextComboAvailable(false);
		Pawn->ComboCheck();

	}
}
