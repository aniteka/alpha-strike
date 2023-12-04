// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerHUD.h"
#include "UI/Player/PlayerWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerWidgetClass && DeathWidgetClass);

	PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
	DeathWidget = CreateWidget(GetWorld(), DeathWidgetClass);
	check(PlayerWidget && DeathWidget);
	
	PlayerWidget->AddToViewport();
}

void APlayerHUD::PopupDeathWidget()
{
	if (!DeathWidget)
		return;
	DeathWidget->AddToViewport(10);
	GetOwningPlayerController()->SetShowMouseCursor(true);
	GetOwningPlayerController()->SetInputMode(FInputModeUIOnly{});
}

void APlayerHUD::CloseDeathWidget()
{
	if (!DeathWidget)
		return;
	DeathWidget->RemoveFromParent();
	GetOwningPlayerController()->SetShowMouseCursor(false);
	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly{});
}
