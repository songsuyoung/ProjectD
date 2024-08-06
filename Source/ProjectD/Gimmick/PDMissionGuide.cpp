// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PDMissionGuide.h"
#include "Interfaces/PDClearPathInterface.h"
#include "ProjectD.h"

APDMissionGuide::APDMissionGuide()
{
    OnActorBeginOverlap.AddDynamic(this,&APDMissionGuide::ActorBeginOverlap);
}

FVector APDMissionGuide::GetPosition()
{
    FVector Location = GetActorLocation(); //���� �ƴ� Box�� ����� ��, ������ �߾��� �ƴϴ�.

    //�ش� ������ �ذ��Ѵ�.

    return Location;
}

void APDMissionGuide::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    //Spline Mesh Ŭ��� ���ؼ� ����Ѵ�.
    PD_LOG(PDLog, Log, TEXT("ActorBegin"));

    IPDClearPathInterface* ClearPathInterface = Cast< IPDClearPathInterface>(OtherActor);

    if (ClearPathInterface)
    {
        ClearPathInterface->ClearPath();
    }
}
