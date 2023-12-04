// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerHUD.h"
#include "UI/Player/PlayerWidget.h"
#include "UI/Player/PauseWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerWidgets.Add(EGameState::InGame, CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass));
	PlayerWidgets.Add(EGameState::Pause, CreateWidget<UPauseWidget>(GetWorld(), PauseWidgetClass));

	if (GetWorld()) {
		const auto GameMode = Cast<AGameModeDM>(GetWorld()->GetAuthGameMode());

		GameMode->OnGameStateChanged.AddUObject(this, &APlayerHUD::OnGameStateChanged);
	}

	for (auto WidgetPair : PlayerWidgets) {
		const auto Widget = WidgetPair.Value;

		if (!Widget) {
			continue;
		}

		Widget->AddToViewport();
		Widget->SetVisibility(ESlateVisibility::Hidden);

	}
}

void APlayerHUD::OnGameStateChanged(EGameState NewState)
{

	if (CurrentWidget) {
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PlayerWidgets.Contains(NewState)) {
		CurrentWidget = PlayerWidgets[NewState];
	}

	if (CurrentWidget) {
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}


}
