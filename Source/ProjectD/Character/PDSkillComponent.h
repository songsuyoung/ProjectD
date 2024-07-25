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
	//Axe�� ���� ��ų Q
	void SkillQByAxe();
	//Axe�� ���� ��ų W
	void SkillWByAxe();
	//Axe�� ���� ��ų 
	void SkillEByAxe();

};
