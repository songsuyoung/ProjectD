// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetStaminaProgressBar(float InCurrentVal, float InMaxVal);
	void SetHPProgressBar(float InCurrentVal, float InMaxVal);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPDStat> WB_HP;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPDStat> WB_Stamina;

};
