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
    FVector Location = GetActorLocation(); //원이 아닌 Box를 사용할 때, 원점이 중앙이 아니다.

    //해당 문제를 해결한다.

    return Location;
}

void APDMissionGuide::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    //Spline Mesh 클리어를 위해서 사용한다.
    PD_LOG(PDLog, Log, TEXT("ActorBegin"));

    IPDClearPathInterface* ClearPathInterface = Cast< IPDClearPathInterface>(OtherActor);

    if (ClearPathInterface)
    {
        ClearPathInterface->ClearPath();
    }
}
