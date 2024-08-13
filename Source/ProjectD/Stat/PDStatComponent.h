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
	virtual void BeginPlay() override;


	float GetHP() { return HP; }
	float GetMaxHP() { return MaxHP; }
	float GetStamina() { return Stamina; }
	float GetMaxStamina() { return MaxStamina; }
	float GetPower() { return Power; }
	float GetMaxPower() { return MaxPower; }

	FORCEINLINE void SetHP(float InHP)
	{
		HP = FMath::Clamp(InHP, 0, MaxHP);

		if (OnUpdatedHP.IsBound())
		{
			OnUpdatedHP.Broadcast(HP, MaxHP);
		}
	}
	FORCEINLINE void SetMaxHP(float InMaxHP)
	{
		MaxHP = FMath::Clamp(InMaxHP, 0, 1000.f);

		if (OnUpdatedHP.IsBound())
		{
			OnUpdatedHP.Broadcast(HP, MaxHP);
		}
	}
	FORCEINLINE void SetStamina(float InStamina)
	{
		Stamina = FMath::Clamp(InStamina, 0, MaxStamina);

		if (OnUpdatedStamina.IsBound())
		{
			OnUpdatedStamina.Broadcast(Stamina, MaxStamina);
		}
	}
	FORCEINLINE void SetMaxStamina(float InMaxStamina)
	{
		MaxStamina = FMath::Clamp(InMaxStamina, 0, 500.f);

		if (OnUpdatedStamina.IsBound())
		{
			OnUpdatedStamina.Broadcast(Stamina, MaxStamina);
		}
	}
	FORCEINLINE void SetPower(float InPower)
	{
		Power = FMath::Clamp(InPower, 0, MaxPower);

		if (OnUpdatedPower.IsBound())
		{
			OnUpdatedPower.Broadcast(Power, MaxPower);
		}
	}
	FORCEINLINE void SetMaxPower(float InMaxPower)
	{
		MaxPower = FMath::Clamp(InMaxPower, 0, 100.f);;

		if (OnUpdatedPower.IsBound())
		{
			OnUpdatedPower.Broadcast(Power, MaxPower);
		}
	}
	
	FOnUpdatedHP OnUpdatedHP;
	FOnUpdatedStamina OnUpdatedStamina;
	FOnUpdatedPower OnUpdatedPower;

protected:
	/*실제 첫 초기화 Stat*/
	void Init(float InMaxHp, float InMaxStamina, float InMaxPower);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float HP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float Power;
};
