// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameData/PDEMissionType.h"
#include "PDDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDDataManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	TObjectPtr<class APDMissionGuide> GetMission(PDEMissionType Type);
protected:

	UPROPERTY()
	TMap<PDEMissionType,TObjectPtr<class APDMissionGuide>> MissionGuide;
};
