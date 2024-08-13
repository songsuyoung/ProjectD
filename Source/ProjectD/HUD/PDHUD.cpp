// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PDHUD.h"
#include "Blueprint/UserWidget.h"
#include "Player/PDPlayerState.h"
#include "Stat/PDStatComponent.h"

#include "UI/PDMainWidget.h"
#include "ProjectD.h"
void APDHUD::CreateUI()
{
    if (HUDClass.Num() == 0)
    {
        PD_LOG(PDLog, Error, TEXT("There is no UI"));
        return;
    }

    //위젯을 생성하는 부분
    for (const auto& HUD : HUDClass) 
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HUD.Value);
        Widget->AddToViewport(0);
        Widget->SetVisibility(ESlateVisibility::Visible);
        HUDs.Add(HUD.Key, Widget);
    }

    /*델리게이트 연결*/
    APDPlayerState* PS = GetOwningPawn()->GetPlayerState<APDPlayerState>();
    if (PS)
    {
        PS->GetStat()->OnUpdatedHP.AddUObject(this, &APDHUD::UpdatedHP);
        PS->GetStat()->OnUpdatedStamina.AddUObject(this, &APDHUD::OnUpdatedStamina);
        PS->GetStat()->OnUpdatedPower.AddUObject(this, &APDHUD::OnUpdatedPower);
    }

}

void APDHUD::BeginPlay()
{
    Super::BeginPlay();

    // HUD 클래스는 클라이언트 상에서만 등장하기 때문에,
    // 데디케이티드 서버에서는 BeginPlay에서 생성이 가능하다.
    // 하지만,리슨 서버상에서는 서버에서 호출이 안된다는 문제점을 가지고 있었다. 
    CreateUI();
}

void APDHUD::SetVisibility(PDEUIType UIType)
{
    if (HUDs.Find(UIType) == nullptr) return;

    UUserWidget* Widget = HUDs[UIType];

    if (Widget)
    {
        Widget->SetVisibility(ESlateVisibility::Visible);
    }
}

void APDHUD::SetHidden(PDEUIType UIType)
{
    if (HUDs.Find(UIType) == nullptr) return;

    UUserWidget* Widget = HUDs[UIType];

    if (Widget)
    {
        Widget->SetVisibility(ESlateVisibility::Collapsed);
        //Collapsed로 할 경우, 공간을 차지하지 않아서 Hidden보다 효율적이다.
    }
}

void APDHUD::UpdatedHP(float InHP, float InMaxHP)
{
    UPDMainWidget* Widget = Cast<UPDMainWidget>(HUDs[PDEUIType::Main]);

    if (Widget)
    {
        //Update HP
        Widget->SetHPProgressBar(InHP, InMaxHP);
    }
}

void APDHUD::OnUpdatedStamina(float InStamina, float InMaxStamina)
{
    UPDMainWidget* Widget = Cast<UPDMainWidget>(HUDs[PDEUIType::Main]);

    if (Widget)
    {
        //Update Stamina
        Widget->SetStaminaProgressBar(InStamina, InMaxStamina);
    }
}

void APDHUD::OnUpdatedPower(float InPower, float InMaxPower)
{
    UUserWidget* Widget = HUDs[PDEUIType::Main];

    if (Widget)
    {
        //Update Power
    }
}

