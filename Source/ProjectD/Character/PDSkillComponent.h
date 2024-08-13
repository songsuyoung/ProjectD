// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/PDESkillType.h"
#include "GameData/PDEWeaponType.h"
#include "PDSkillComponent.generated.h"


/*Delegate Skill에 대해 선언*/
DECLARE_DELEGATE(FOnAttackSkillDelegate);
/*실제 공격을 수행할 델리게이트*/
DECLARE_DELEGATE(FOnComboAttackDelegate);
USTRUCT(BlueprintType)
struct FAttackSkillDelegateWrapper
{
	GENERATED_BODY()

public:
	FAttackSkillDelegateWrapper() { }
	FAttackSkillDelegateWrapper(PDESkillType SkillType, const FOnAttackSkillDelegate& InAttackSkillDelegate) { AttackSkillDelegate.Add(SkillType, InAttackSkillDelegate); }
	TMap<PDESkillType, FOnAttackSkillDelegate> AttackSkillDelegate;
};

USTRUCT(BlueprintType)
struct FOnComboAttackDelegateWrapper
{
	GENERATED_BODY()

public:
	FOnComboAttackDelegateWrapper() { }
	FOnComboAttackDelegateWrapper(const FOnComboAttackDelegate& InOnComboAttackDelegate) { OnComboAttackDelegate = InOnComboAttackDelegate; }
	FOnComboAttackDelegate OnComboAttackDelegate;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UPDSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPDSkillComponent();

	TMultiMap< PDEWeaponType, FAttackSkillDelegateWrapper> AttackDelegate;

	int8 ComboAttackIndex;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:

	FTimerHandle ComboTimerHandleByAxe;

	void AttackBaseByAxe();
	//Axe에 의한 스킬 Q
	void SkillQByAxe();
	//Axe에 의한 스킬 W
	void SkillWByAxe();
	//Axe에 의한 스킬 
	void SkillEByAxe();

	void AttackBaseBySword();
	//Sword에 의한 스킬 Q
	void SkillQBySword();
	//Sword에 의한 스킬 W
	void SkillWBySword();
	//Sword에 의한 스킬 
	void SkillEBySword();

	void AttackBaseByBow();
	//Bow에 의한 스킬 Q
	void SkillQByBow();
	//Bow에 의한 스킬 W
	void SkillWByBow();
	//Bow에 의한 스킬 
	void SkillEByBow();

protected:
	//RPC
	UFUNCTION(Server,Reliable)
	void ServerRPCBaseSkill();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPCBaseSkill();

};
