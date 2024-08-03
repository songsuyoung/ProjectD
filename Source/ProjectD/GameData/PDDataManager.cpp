// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/PDDataManager.h"
#include "Gimmick/PDMissionGuide.h"
#include "EngineUtils.h"

/*World가 다 준비 됐을 때 모든 값을 가져온다.*/
void UPDDataManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	UWorld* World = GetWorld();

	for (const auto& Entry : FActorRange(World))
	{
		APDMissionGuide* Mission = Cast<APDMissionGuide>(Entry);

		if (Mission)
		{
			UE_LOG(LogTemp, Log, TEXT("Mission %d"), Mission->GetType());
			MissionGuide.Add(Mission->GetType(),Mission);
		}
	}
}

TObjectPtr<class APDMissionGuide> UPDDataManager::GetMission(PDEMissionType Type)
{
	if (MissionGuide.Find(Type) == nullptr)
	{
		return nullptr;
	}
	
	return MissionGuide[Type];
}
