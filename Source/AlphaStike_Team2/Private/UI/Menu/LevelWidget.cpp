// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/LevelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void ULevelWidget::SetupLevel(const FLevelData& Data)
{
	LevelData = Data;

	if (LevelImage) {
		LevelImage->SetBrushFromTexture(LevelData.LevelImage);
	}

	if (LevelName) {
		LevelName->SetText(FText::FromName(LevelData.LevelName));
	}

	if (LevelDescription) {
		LevelDescription->SetText(FText::FromName(LevelData.LevelDescription));
	}

}

void ULevelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SelectLevelButton) {
		SelectLevelButton->OnClicked.AddDynamic(this, &ULevelWidget::OnSelectLevelButtonClicked);
	}
}

void ULevelWidget::OnSelectLevelButtonClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}
