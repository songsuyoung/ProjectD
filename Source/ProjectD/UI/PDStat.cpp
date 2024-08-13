// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDStat.h"
#include "Components/ProgressBar.h"
#include "Interfaces/PDWidgetInterface.h"
#include "Components/TextBlock.h"

UPDStat::UPDStat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPDStat::NativeConstruct()
{
	Super::NativeConstruct();

	IPDWidgetInterface* WidgetInterface = Cast<IPDWidgetInterface>(Owner);
	if (WidgetInterface)
	{
		WidgetInterface->InitWidget(this);
	}
}

void UPDStat::SetProgressBar(float InCurrentVal, float InMaxVal)
{
	ProgressBar->SetPercent(InCurrentVal / InMaxVal);

	SetPercentage(InCurrentVal, InMaxVal);
}

void UPDStat::SetPercentage(float InCurrentVal, float InMaxVal)
{
	/*소수점을 버린다. .0f*/
	FString TxtPer = FString::Printf(TEXT("%.0f/%.0f"),InCurrentVal,InMaxVal); //소수점 제거

	TxtPercentage->SetText(FText::FromString(TxtPer));
}
