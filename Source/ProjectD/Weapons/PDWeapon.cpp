// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PDWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Stat/PDStatComponent.h"
#include "GameData/PDWeaponData.h"
#include "Physics/PDCollision.h"

#include "ProjectD.h"

// Sets default values
APDWeapon::APDWeapon()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	//Default
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/LowPolyWeapons/Meshes/SM_Axe1.SM_Axe1'"));

	if (StaticMeshRef.Object)
	{
		Mesh->SetStaticMesh(StaticMeshRef.Object);
	}

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(CPROFILE_PDATTACK);
	Collision->SetRelativeLocation(FVector(10.f, 0.f, 100.f));
	Collision->SetBoxExtent(FVector(20.f, 10.f, 15.f));
	Collision->SetupAttachment(Mesh);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APDWeapon::OnComponentBeginOverlap);
}

void APDWeapon::Equip(ACharacter* Character)
{
	if (Character)
	{
		SetOwner(Character);
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("weapon"));
		this->SetActorRelativeRotation(FRotator(0.f, 180.f, 0.0f));
	}
}

void APDWeapon::UnEquip()
{
	ACharacter* Character = GetOwner<ACharacter>();
	if (Character)
	{
		//DetachÇÑ´Ù.
		Character->GetMesh();
		SetOwner(nullptr);
	}

}

int APDWeapon::GetMaxComboLen()
{
	return WeaponData->MaxComboLen;
}

float APDWeapon::GetPlayRate()
{
	return 1.0f;
}

PDEWeaponType APDWeapon::GetWeaponType()
{
	return WeaponData->WeaponType;
}

UAnimMontage* APDWeapon::GetAnimMontage()
{
	return WeaponData->AttackMontage;
}

void APDWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsAttacking)
	{
		PD_LOG(PDLog, Log, TEXT("OnComponentBeginOverlap"));
	}
}


