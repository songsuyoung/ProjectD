// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PDHPWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDHPWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget() override;

};
