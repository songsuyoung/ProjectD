// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void SetOwner(class AActor* InOwner) { Owner = InOwner; }
protected:

	UPROPERTY()
	TObjectPtr<class AActor> Owner;
};
