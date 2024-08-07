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
	void ComboBegin();
	void ComboNext();

	UFUNCTION()
	void ComboEnd();
	void PlayAttackAnimation();
	void ComboInit();

	UFUNCTION(Client, Reliable)
	void ClientRPCBaseSkill(class APDCharacterPlayer* OtherPlayer);

	UFUNCTION()
	void OnRep_CanAttack();
protected:

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;	
	
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const;


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
