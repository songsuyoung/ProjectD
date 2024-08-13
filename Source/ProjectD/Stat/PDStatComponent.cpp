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
	/*���� �ʱ�ȭ�� �������ش�. ����� �����ִ� ��������, ������ �ҷ��ͼ� �ʱ�ȭ ����*/
	SetMaxHP(InMaxHp);
	SetHP(MaxHP);

	SetMaxStamina(InMaxStamina);
	SetStamina(MaxStamina);

	SetMaxPower(InMaxPower);
	SetPower(MaxPower);
}

