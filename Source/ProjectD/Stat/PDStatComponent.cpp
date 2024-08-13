// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PDStatComponent.h"

// Sets default values for this component's properties
UPDStatComponent::UPDStatComponent()
	: Level(1.f), MaxHP(100.f),HP(MaxHP),MaxStamina(50.f),Stamina(MaxStamina),MaxPower(30.f),Power(MaxPower)
{
	
}

void UPDStatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init(100.f, 50.f, 30.f);
}

void UPDStatComponent::Init(float InMaxHp, float InMaxStamina, float InMaxPower)
{
	/*스탯 초기화를 수행해준다. 현재는 박혀있는 값이지만, 엑셀을 불러와서 초기화 예정*/
	SetMaxHP(InMaxHp);
	SetHP(MaxHP);

	SetMaxStamina(InMaxStamina);
	SetStamina(MaxStamina);

	SetMaxPower(InMaxPower);
	SetPower(MaxPower);
}

