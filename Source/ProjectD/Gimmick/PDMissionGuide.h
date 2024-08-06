// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/PDEMissionType.h"
#include "Interfaces/PDPostMissionInterface.h"
#include "Engine/TriggerVolume.h"
#include "PDMissionGuide.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDMissionGuide : public ATriggerVolume, public IPDPostMissionInterface
{
	GENERATED_BODY()

public:
	APDMissionGuide();

	PDEMissionType GetType() { return Type; }

	virtual FVector GetPosition() override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	PDEMissionType Type;

	/*이후 트리거 됐을 때 호출할 함수 작성*/
	UFUNCTION()
	void ActorBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);
};
