// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerHUD.h"
#include "UI/Player/PlayerWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidgetClass) {
		auto Widget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

		if (Widget) {
			Widget->AddToViewport();
		}
	}
}
