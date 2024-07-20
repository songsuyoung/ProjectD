// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PDCharacterBase.h"
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

	/*Mouse Zoom -> Camera ZoomIn/ZoomOut 구현 예정*/
protected:

	/*Input*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPDInputComponent> PDInputComponent;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

protected:
	/*Defualt Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerController> PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APDPlayerState> PS;

};
