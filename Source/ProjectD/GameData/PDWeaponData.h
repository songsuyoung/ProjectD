// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameData/PDEWeaponType.h"
#include "PDWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/*GameData·Î ¹­ÀÚ*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	PDEWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int MaxComboLen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UAnimMontage> AttackMontage;

};
