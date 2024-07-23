// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDStat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPDStat::SetProgressBar(float InCurrentVal, float InMaxVal)
{
	ProgressBar->SetPercent(InCurrentVal / InMaxVal);
}

void UPDStat::SetPercentage(float InCurrentVal, float InMaxVal)
{
	FString TxtPer = FString::Printf(TEXT("%d/%d"),InCurrentVal,InMaxVal); //소수점 제거

	TxtPercentage->SetText(FText::FromString(TxtPer));
}
