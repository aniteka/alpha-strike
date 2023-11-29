// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"
#include "MenuWidget.generated.h"

class UButton;
class UVerticalBox;

UCLASS()
class ALPHASTIKE_TEAM2_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized()override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LevelWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PreloaderAnimation;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LevelsVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectLevelButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LevelsMenuAnimation;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)override;

private:

	void CreateLevels();
	void InitializeFirstLevel();
	void SetupLevel(const FLevelData& Data);

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	UFUNCTION()
	void OnSelectLevel();

	FLevelData StartLevelData;

	UMainGameInstance* GetGameInstance()const;

};
