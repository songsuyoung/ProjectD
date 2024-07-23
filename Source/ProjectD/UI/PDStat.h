// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDStat : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetProgressBar(float InCurrentVal, float InMaxVal);
	void SetPercentage(float InCurrentVal, float InMaxVal);
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtPercentage;
};
