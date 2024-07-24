// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PDHPWidgetComponent.h"
#include "UI/PDUserWidget.h"

void UPDHPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//지정된 WidgetClass를 가져옴
	//Widget에는 Owner가 없어서 값을 쉽게 못변경함.
	//인게임 내에 있는 HP 값을 변화시키기 위해서 오너를 설정해준다.
	UPDUserWidget* UserWidget = Cast<UPDUserWidget>(GetWidget());

	if (UserWidget)
	{
		UserWidget->SetOwner(GetOwner());
	}
}
