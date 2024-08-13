// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDMainWidget.h"
#include "UI/PDStat.h"

void UPDMainWidget::SetStaminaProgressBar(float InCurrentVal, float InMaxVal)
{
	WB_Stamina->SetProgressBar(InCurrentVal, InMaxVal);
}

void UPDMainWidget::SetHPProgressBar(float InCurrentVal, float InMaxVal)
{
	WB_HP->SetProgressBar(InCurrentVal, InMaxVal);
}

