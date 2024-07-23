// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PDPlayerState.h"
#include "Stat/PDStatComponent.h"

APDPlayerState::APDPlayerState()
{
	Stat = CreateDefaultSubobject<UPDStatComponent>(TEXT("StatComponent"));
}

void APDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//Stat - UI HUD에서 연결해주자.
}
