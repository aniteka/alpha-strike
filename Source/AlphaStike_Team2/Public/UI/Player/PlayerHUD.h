// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameModes/GameModeDM.h"
#include "PlayerHUD.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	void OnGameStateChanged(EGameState NewState);

private:

	UUserWidget* CurrentWidget = nullptr;
	TMap<EGameState, UUserWidget*> PlayerWidgets;
};
