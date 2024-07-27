// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PDCharacterBase.h"
#include "GameData/PDEWeaponType.h"
#include "GameData/PDESkillType.h"
#include "PDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDCharacterPlayer : public APDCharacterBase
{
	GENERATED_BODY()
	
public:

	APDCharacterPlayer();

protected:
	/*Default Function*/
	virtual void PossessedBy(AController* NewController) override; //Server Only
	virtual void OnRep_Controller() override; //Client Only
	virtual void BeginPlay() override;
protected:
	/*Spring Arm, Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

public:

	USpringArmComponent* GetSpringArm();
	/*Spring Arm Length*/
	int SpringArmLength();

	/*Mouse Zoom -> Camera ZoomIn/ZoomOut ���� ����*/
protected:

	/*Input*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPDInputComponent> PDInputComponent;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*����*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class APDWeapon> Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class APDWeapon> DefaultWeaponClass;

	/*Attack�� ���� ���� Component�� ����, ���� Weapon�� �����ϰ� ��������*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UPDSkillComponent> AttackComponent;

public:
	void Skill(PDESkillType SkillType);

	void IncreaseComboIdx();
	void DecreaseComboIdx();

protected:
	/*Defualt Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerController> PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerState> PS;

	/*Widget Component ����*/
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UPDHPWidgetComponent> HPWidget;

public:
	/*Animation*/

	void PlayAnimation(FName MontageSection);

	void MontageJumpToSection(FName MontageSection);

	UFUNCTION(Server, Reliable)
	void ServerRPCAttackAnimation(FName MontageSection);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCAttackAnimation(FName MontageSection);

};
