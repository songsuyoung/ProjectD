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

USTRUCT(BlueprintType)
struct FAttackSkillDelegateWrapper
{
	GENERATED_BODY()

public:
	FAttackSkillDelegateWrapper() { }
	FAttackSkillDelegateWrapper(PDESkillType SkillType, const FOnAttackSkillDelegate& InAttackSkillDelegate) { AttackSkillDelegate.Add(SkillType, InAttackSkillDelegate); }
	TMap<PDESkillType, FOnAttackSkillDelegate> AttackSkillDelegate;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UPDSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPDSkillComponent();

	TMultiMap< PDEWeaponType, FAttackSkillDelegateWrapper> AttackDelegate;

protected:

	void AttackBaseByAxe();
	//Axe에 의한 스킬 Q
	void SkillQByAxe();
	//Axe에 의한 스킬 W
	void SkillWByAxe();
	//Axe에 의한 스킬 
	void SkillEByAxe();

};
