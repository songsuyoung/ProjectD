// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacterBase.h"
#include "Physics/PDCollision.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APDCharacterBase::APDCharacterBase()
{	
	//Pawn - ���ͺ� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->JumpZVelocity = 0.f; //���� �� �� ����.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.0f, 0.f); //FORCEINLINE FRotator(T InPitch, T InYaw, T InRoll);
	GetCharacterMovement()->AirControl = 0.35f; //������ ���⶧���� ���߿� �� Ȯ���� ������, ���� ���� ����.
	GetCharacterMovement()->bOrientRotationToMovement = true; //ĳ���ͳ� ���Ͱ� �̵��� �� �� �������� ȸ���� ����(���콺�� ���� ȸ���� �����ϵ��� ������)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; //���� ���ߴ� ȿ�� 
	GetCharacterMovement()->bWantsToCrouch = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(25.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PDCAPSULE); //���߿� ���� ����
	
	//SkeletalMesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.0f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}