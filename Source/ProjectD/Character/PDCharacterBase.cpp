// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacterBase.h"
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 120.0f, 0.f); //FORCEINLINE FRotator(T InPitch, T InYaw, T InRoll);
	GetCharacterMovement()->AirControl = 0.35f; //������ ���⶧���� ���߿� �� Ȯ���� ������, ���� ���� ����.
	GetCharacterMovement()->bOrientRotationToMovement = true; //ĳ���ͳ� ���Ͱ� �̵��� �� �� �������� ȸ���� ����(���콺�� ���� ȸ���� �����ϵ��� ������)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; //���� ���ߴ� ȿ�� 
	GetCharacterMovement()->bWantsToCrouch = false;

	//CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision")); //���߿� ���� ����
	
	//SkeletalMesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.0f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//�ϴ�, �𸮾��� �⺻ ĳ���ͷ� ���� ����
}

// Called when the game starts or when spawned
void APDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

