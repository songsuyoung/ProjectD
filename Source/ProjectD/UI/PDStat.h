// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PDUserWidget.h"
#include "PDStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDStat : public UPDUserWidget
{
	GENERATED_BODY()
	
public:

	UPDStat(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void SetProgressBar(float InCurrentVal, float InMaxVal);
	void SetPercentage(float InCurrentVal, float InMaxVal);
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtPercentage;
};
