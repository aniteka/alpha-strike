// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class ALPHASTIKE_TEAM2_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartLevelButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnContinueGame();

	UFUNCTION()
	void OnMenu();

	UFUNCTION()
	void OnRestartLevel();
};
