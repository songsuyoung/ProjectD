// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDHPWidgetComponent.h"
#include "UI/PDUserWidget.h"

void UPDHPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//������ WidgetClass�� ������
	//Widget���� Owner�� ��� ���� ���� ��������.
	//�ΰ��� ���� �ִ� HP ���� ��ȭ��Ű�� ���ؼ� ���ʸ� �������ش�.
	UPDUserWidget* UserWidget = Cast<UPDUserWidget>(GetWidget());

	if (UserWidget)
	{
		UserWidget->SetOwner(GetOwner());
	}
}
