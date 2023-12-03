// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ContinueButton) {
		ContinueButton->OnClicked.AddDynamic(this, &UPauseWidget::OnContinueGame);
	}

	if (MenuButton) {
		MenuButton->OnClicked.AddDynamic(this, &UPauseWidget::OnMenu);
	}

	if (RestartLevelButton) {
		RestartLevelButton->OnClicked.AddDynamic(this, &UPauseWidget::OnRestartLevel);
	}
}

void UPauseWidget::OnContinueGame()
{
	if (!GetWorld()) {
		return;
	}

	const auto GameMode = GetWorld()->GetAuthGameMode();
	if (!GameMode) {
		return;
	}

	GameMode->ClearPause();

}

void UPauseWidget::OnMenu()
{
	if (!GetWorld()) {
		return;
	}

	const auto GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance) {
		return;
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetMenuLevelName());
}

void UPauseWidget::OnRestartLevel()
{
	if (!GetWorld()) {
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
	
}
