// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameData/PDEUIType.h"
#include "PDHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDHUD : public AHUD
{
	GENERATED_BODY()

public:
	void CreateUI();

	virtual void BeginPlay() override;

	void SetVisibility(PDEUIType UIType);
	void SetHidden(PDEUIType UIType);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TMap<PDEUIType, TObjectPtr<class UUserWidget>> HUDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TMap<PDEUIType, TSubclassOf<class UUserWidget>> HUDClass;

protected:
	/*Callback Func*/
	
	UFUNCTION()
	void UpdatedHP(float InHP, float InMaxHP);

	UFUNCTION()
	void OnUpdatedStamina(float InStamina, float InMaxStamina);

	UFUNCTION()
	void OnUpdatedPower(float InPower, float InMaxPower);
};
