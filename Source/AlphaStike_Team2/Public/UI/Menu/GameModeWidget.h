// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"
#include "GameModeWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameModeSelected, FName);

UCLASS()
class ALPHASTIKE_TEAM2_API UGameModeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupGameModeData(const FGameModeData& Data);

	FOnGameModeSelected OnGameModeSelected;

protected:
	virtual void NativeOnInitialized()override;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectGameModeButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameModeName;

private:
	UFUNCTION()
	void OnSelectGameModeButtonClicked();

	FGameModeData GameModeData;
};
