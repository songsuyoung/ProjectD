// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PDWidgetInterface.h"
#include "PDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDPlayerState : public APlayerState, public IPDWidgetInterface
{
	GENERATED_BODY()	
public:
	APDPlayerState();

	virtual void BeginPlay() override;

	class UPDStatComponent* GetStat() { return Stat; }

	virtual void InitWidget(class UPDUserWidget* StatWidget) override;
private:
	//Player Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPDStatComponent> Stat;
};
