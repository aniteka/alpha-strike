// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"
#include "LevelWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelected,const FLevelData&);

UCLASS()
class ALPHASTIKE_TEAM2_API ULevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupLevel(const FLevelData& Data);
	
	FOnLevelSelected OnLevelSelected;

protected:
	virtual void NativeOnInitialized()override;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectLevelButton;

	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelName;

private:
	UFUNCTION()
	void OnSelectLevelButtonClicked();

	FLevelData LevelData;
};
