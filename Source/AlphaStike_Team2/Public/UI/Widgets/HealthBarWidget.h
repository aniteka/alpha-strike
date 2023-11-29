// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UHealthComponent;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ALPHASTIKE_TEAM2_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeWidget(UHealthComponent* HealthComp);
protected:
	UHealthComponent* HealthComponent;

	UPROPERTY (EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentHealthLabel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MaxHealthLabel;

	void OnHealthUpdate(float value);
};
