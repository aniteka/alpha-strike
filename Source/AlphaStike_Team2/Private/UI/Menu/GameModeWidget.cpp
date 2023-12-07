// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/GameModeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGameModeWidget::SetupGameModeData(const FGameModeData& Data)
{
	GameModeData = Data;

	if (GameModeName) {
		GameModeName->SetText(FText::FromName(Data.GameModeName));
	}
}

void UGameModeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SelectGameModeButton) {
		SelectGameModeButton->OnClicked.AddDynamic(this, &UGameModeWidget::OnSelectGameModeButtonClicked);
	}
}

void UGameModeWidget::OnSelectGameModeButtonClicked()
{
	OnGameModeSelected.Broadcast(GameModeData.GameModeName);
}
