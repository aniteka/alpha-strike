// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlphaStrikeTypes.h"
#include "TeamWidget.generated.h"

class UButton;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamSelected,UTeamWidget*)

UCLASS()
class ALPHASTIKE_TEAM2_API UTeamWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupTeamData(ETeamType NewTeamType);

	FOnTeamSelected OnTeamSelected;

protected:
	virtual void NativeOnInitialized()override;

	UPROPERTY(meta = (BindWidget))
	UButton* TeamButton;

	UPROPERTY(meta = (BindWidget))
	UImage* TeamImage;

private:
	UFUNCTION()
	void OnTeamButtonClicked();

	ETeamType TeamType = ETeamType::None;

};
