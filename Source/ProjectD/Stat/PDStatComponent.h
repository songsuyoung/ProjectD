// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PDStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdatedHP, float, float); /*HP UI*/
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdatedStamina, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdatedPower, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UPDStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPDStatComponent();

	FORCEINLINE void SetHP(float InHP)
	{
		HP = InHP;

		if (OnUpdatedHP.IsBound())
		{
			OnUpdatedHP.Broadcast(HP, MaxHP);
		}
	}
	FORCEINLINE void SetMaxHP(float InMaxHP)
	{
		MaxHP = InMaxHP;

		if (OnUpdatedHP.IsBound())
		{
			OnUpdatedHP.Broadcast(HP, MaxHP);
		}
	}
	FORCEINLINE void SetStamina(float InStamina)
	{
		Stamina = InStamina;

		if (OnUpdatedStamina.IsBound())
		{
			OnUpdatedStamina.Broadcast(Stamina, MaxStamina);
		}
	}
	FORCEINLINE void SetMaxStamina(float InMaxStamina)
	{
		MaxStamina = InMaxStamina;

		if (OnUpdatedStamina.IsBound())
		{
			OnUpdatedStamina.Broadcast(Stamina, MaxStamina);
		}
	}
	FORCEINLINE void SetPower(float InPower)
	{
		Power = InPower;

		if (OnUpdatedPower.IsBound())
		{
			OnUpdatedPower.Broadcast(Power, MaxPower);
		}
	}
	FORCEINLINE void SetMaxPower(float InMaxPower)
	{
		MaxPower = InMaxPower;

		if (OnUpdatedPower.IsBound())
		{
			OnUpdatedPower.Broadcast(Power, MaxPower);
		}
	}
	
	FOnUpdatedHP OnUpdatedHP;
	FOnUpdatedStamina OnUpdatedStamina;
	FOnUpdatedPower OnUpdatedPower;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float HP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Power;
};
