// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDPathNiagaraComponent.h"

UPDPathNiagaraComponent::UPDPathNiagaraComponent()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));

	if (NiagaraRef.Object)
	{
		//SetAsset(NiagaraRef.Object);
	}
}
