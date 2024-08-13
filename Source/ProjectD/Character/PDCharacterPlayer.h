// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PDCharacterBase.h"
#include "GameData/PDEWeaponType.h"
#include "GameData/PDESkillType.h"
#include "Interfaces/PDClearPathInterface.h"
#include "PDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDCharacterPlayer : public APDCharacterBase, public IPDClearPathInterface
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

	/*Mouse Zoom -> Camera ZoomIn/ZoomOut 구현 예정*/
protected:

	/*Input*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPDInputComponent> PDInputComponent;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*무기*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class APDWeapon> Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class APDWeapon> DefaultWeaponClass;

	/*Attack에 대한 공격 Component를 만들어서, 여러 Weapon에 유연하게 대응하자*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UPDSkillComponent> AttackComponent;

public:
	void Skill(PDESkillType SkillType);
	void PlayAttackAnimation();
	void PlayMontageNotifyBegin();
	
	UFUNCTION(Server,Reliable)
	void ServerRPCEndedAttack();

	/*Attack?*/
	bool GetFinalComboAttack() { return bIsFinalComboAttack; }
	bool GetAttacking() { return bIsAttacking; }
protected:

	UPROPERTY(Meta = (AllowPrivateAccess = "true"))
	uint8 bIsAttacking : 1;
	/*현재 공격중인지를 체크한다. 현재 공격 중일 때에는 이동하지 못하게 막기 위해서*/

	UPROPERTY(Meta = (AllowPrivateAccess = "true"))
	uint8 bIsFinalComboAttack : 1;
	/*마지막 콤보 공격인지를 확인한다. 쓸데 없는 RPC 전송을 막기 위해서*/
protected:
	/*Defualt Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerController> PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerState> PS;

	/*Widget Component 생성*/
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UPDHPWidgetComponent> HPWidget;

public:

	/*길을 표현하기 위해서 사용*/
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	/*미션 공간에 닿았을 때 Path구역이 사라진다.*/
	virtual void ClearPath() override;

protected:

	/*서버는 없어도, 클라이언트만 생성으로 서버의 부하를 줄인다.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Path)
	TObjectPtr<class USplineComponent> Path;

	/*이후 재사용성을 생각해보자*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Path)
	TArray<TObjectPtr<class USplineMeshComponent>> PathMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Path)
	TSubclassOf<class USplineMeshComponent> PathMeshClass;

	void SetPath();
	void InitPath();
	void DrawPath(int PathLen);
	
};
