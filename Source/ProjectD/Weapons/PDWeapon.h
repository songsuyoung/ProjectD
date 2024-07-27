// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/PDEWeaponType.h"
#include "PDWeapon.generated.h"

UCLASS()
class PROJECTD_API APDWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDWeapon();

	void Equip(class ACharacter *Character);
	void UnEquip();
	int GetMaxComboLen();
	float GetPlayRate();
	PDEWeaponType GetWeaponType();
	class UAnimMontage* GetAnimMontage();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UBoxComponent> Collision;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	uint8 bIsAttacking : 1;
	
	/*GameData·Î ¹­ÀÚ*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UPDWeaponData> WeaponData;
};
