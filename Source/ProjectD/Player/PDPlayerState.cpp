// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PDPlayerState.h"
#include "Stat/PDStatComponent.h"
#include "UI/PDUserWidget.h"
#include "UI/PDStat.h"

APDPlayerState::APDPlayerState()
{
	Stat = CreateDefaultSubobject<UPDStatComponent>(TEXT("StatComponent"));
}

void APDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//Stat - UI HUD에서 연결해주자.
}

void APDPlayerState::InitWidget(UPDUserWidget* StatWidget)
{
	UPDStat* UIStat = Cast<UPDStat>(StatWidget);

	if (UIStat)
	{
		UIStat->SetProgressBar(Stat->GetHP(), Stat->GetMaxHP());

		Stat->OnUpdatedHP.AddUObject(UIStat, &UPDStat::SetProgressBar);
	}
}