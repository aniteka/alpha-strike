// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWidget.h"
#include "GameFramework/HUD.h"
#include "GameModes/GameModeDM.h"
#include "PlayerHUD.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

public:
	void PopupDeathWidget();
	void CloseDeathWidget();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;

private:
	UPROPERTY()
	UPlayerWidget* PlayerWidget;
	UPROPERTY()
	UUserWidget* DeathWidgetPopup;

	void OnGameStateChanged(EGameState NewState);

private:

	UUserWidget* CurrentWidget = nullptr;
	TMap<EGameState, UUserWidget*> PlayerWidgets;
};
