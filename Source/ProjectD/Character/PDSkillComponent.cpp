// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDSkillComponent.h"

#include "ProjectD.h"

// Sets default values for this component's properties
UPDSkillComponent::UPDSkillComponent()
{
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::Base, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::AttackBaseByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillQ, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillQByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillW, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillWByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillE, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillEByAxe)));
}

void UPDSkillComponent::AttackBaseByAxe()
{
	PD_SUBLOG(PDLog, Log, TEXT("Base Attack"));
}

void UPDSkillComponent::SkillQByAxe()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillQ"));
}

void UPDSkillComponent::SkillWByAxe()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillW"));
}

void UPDSkillComponent::SkillEByAxe()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillE"));
}



