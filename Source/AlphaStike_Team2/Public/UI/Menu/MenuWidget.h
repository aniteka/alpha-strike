// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;

UCLASS()
class ALPHASTIKE_TEAM2_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized()override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PreloaderAnimation;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)override;

private:

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

};
