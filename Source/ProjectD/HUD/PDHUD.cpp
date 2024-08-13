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

    //������ �����ϴ� �κ�
    for (const auto& HUD : HUDClass) 
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HUD.Value);
        Widget->AddToViewport(0);
        Widget->SetVisibility(ESlateVisibility::Visible);
        HUDs.Add(HUD.Key, Widget);
    }

    /*��������Ʈ ����*/
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

    // HUD Ŭ������ Ŭ���̾�Ʈ �󿡼��� �����ϱ� ������,
    // ��������Ƽ�� ���������� BeginPlay���� ������ �����ϴ�.
    // ������,���� �����󿡼��� �������� ȣ���� �ȵȴٴ� �������� ������ �־���. 
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
        //Collapsed�� �� ���, ������ �������� �ʾƼ� Hidden���� ȿ�����̴�.
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

