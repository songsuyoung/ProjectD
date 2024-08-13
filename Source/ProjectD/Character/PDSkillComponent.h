// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/PDESkillType.h"
#include "GameData/PDEWeaponType.h"
#include "PDSkillComponent.generated.h"


/*Delegate Skill�� ���� ����*/
DECLARE_DELEGATE(FOnAttackSkillDelegate);
/*���� ������ ������ ��������Ʈ*/
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
	//Axe�� ���� ��ų Q
	void SkillQByAxe();
	//Axe�� ���� ��ų W
	void SkillWByAxe();
	//Axe�� ���� ��ų 
	void SkillEByAxe();

	void AttackBaseBySword();
	//Sword�� ���� ��ų Q
	void SkillQBySword();
	//Sword�� ���� ��ų W
	void SkillWBySword();
	//Sword�� ���� ��ų 
	void SkillEBySword();

	void AttackBaseByBow();
	//Bow�� ���� ��ų Q
	void SkillQByBow();
	//Bow�� ���� ��ų W
	void SkillWByBow();
	//Bow�� ���� ��ų 
	void SkillEByBow();

protected:
	//RPC
	UFUNCTION(Server,Reliable)
	void ServerRPCBaseSkill();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPCBaseSkill();

};
