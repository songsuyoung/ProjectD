// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDHPWidgetComponent.h"
#include "UI/PDUserWidget.h"
#include "GameFramework/Character.h"

void UPDHPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//������ WidgetClass�� ������
	//Widget���� Owner�� ��� ���� ���� ��������.
	//�ΰ��� ���� �ִ� HP ���� ��ȭ��Ű�� ���ؼ� ���ʸ� �������ش�.
	UPDUserWidget* UserWidget = Cast<UPDUserWidget>(GetWidget());

	ACharacter* Pawn = Cast<ACharacter>(GetOwner());
	if (UserWidget)
	{
		UserWidget->SetOwner(Pawn->GetPlayerState());
	}
}
