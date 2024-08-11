// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDSkillComponent.h"
#include "Character/PDCharacterPlayer.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectD.h"

// Sets default values for this component's properties
UPDSkillComponent::UPDSkillComponent()
{
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::Base, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::AttackBaseByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillQ, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillQByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillW, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillWByAxe)));
	AttackDelegate.Add(PDEWeaponType::Axe, FAttackSkillDelegateWrapper(PDESkillType::SkillE, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillEByAxe)));


	AttackDelegate.Add(PDEWeaponType::Sword, FAttackSkillDelegateWrapper(PDESkillType::Base, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::AttackBaseBySword)));
	AttackDelegate.Add(PDEWeaponType::Sword, FAttackSkillDelegateWrapper(PDESkillType::SkillQ, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillQBySword)));
	AttackDelegate.Add(PDEWeaponType::Sword, FAttackSkillDelegateWrapper(PDESkillType::SkillW, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillWBySword)));
	AttackDelegate.Add(PDEWeaponType::Sword, FAttackSkillDelegateWrapper(PDESkillType::SkillE, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillEBySword)));


	AttackDelegate.Add(PDEWeaponType::Bow, FAttackSkillDelegateWrapper(PDESkillType::Base, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::AttackBaseByBow)));
	AttackDelegate.Add(PDEWeaponType::Bow, FAttackSkillDelegateWrapper(PDESkillType::SkillQ, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillQByBow)));
	AttackDelegate.Add(PDEWeaponType::Bow, FAttackSkillDelegateWrapper(PDESkillType::SkillW, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillWByBow)));
	AttackDelegate.Add(PDEWeaponType::Bow, FAttackSkillDelegateWrapper(PDESkillType::SkillE, FOnAttackSkillDelegate::CreateUObject(this, &UPDSkillComponent::SkillEByBow)));

	ComboAttackIndex = 0;
}

void UPDSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AttackDelegate.Remove(PDEWeaponType::Axe);
	AttackDelegate.Remove(PDEWeaponType::Sword);
	AttackDelegate.Remove(PDEWeaponType::Bow);
}

FName UPDSkillComponent::GetNextSection(int MaxLen)
{
	ComboAttackIndex = FMath::Clamp(ComboAttackIndex + 1, 1, MaxLen);
	FName NextPunchSection = *FString::Printf(TEXT("Combo%d"), ComboAttackIndex);

	return NextPunchSection;
}

void UPDSkillComponent::AttackBaseByAxe()
{
	ServerRPCBaseSkill();

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

void UPDSkillComponent::AttackBaseBySword()
{
	PD_SUBLOG(PDLog, Log, TEXT("Base Attack"));
}

void UPDSkillComponent::SkillQBySword()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillQ"));
}

void UPDSkillComponent::SkillWBySword()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillW"));
}

void UPDSkillComponent::SkillEBySword()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillE"));
}

void UPDSkillComponent::AttackBaseByBow()
{
	PD_SUBLOG(PDLog, Log, TEXT("Base Attack"));
}

void UPDSkillComponent::SkillQByBow()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillQ"));
}

void UPDSkillComponent::SkillWByBow()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillW"));
}

void UPDSkillComponent::SkillEByBow()
{
	PD_SUBLOG(PDLog, Log, TEXT("Attack SkillE"));
}

void UPDSkillComponent::ResetAttack()
{
	//Pawn->ComboEnd();
}

void UPDSkillComponent::ServerRPCBaseSkill_Implementation()
{
	MulticastRPCBaseSkill();
}

void UPDSkillComponent::MulticastRPCBaseSkill_Implementation()
{
	APDCharacterPlayer *Player = Cast<APDCharacterPlayer>(GetOwner());

	if (Player)
	{
		Player->PlayAttackAnimation();
	}

}