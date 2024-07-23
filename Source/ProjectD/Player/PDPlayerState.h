// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDPlayerState : public APlayerState
{
	GENERATED_BODY()	
public:
	APDPlayerState();

	virtual void BeginPlay() override;

	class UPDStatComponent* GetStat() { return Stat; }
private:
	//Player Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPDStatComponent> Stat;
};
