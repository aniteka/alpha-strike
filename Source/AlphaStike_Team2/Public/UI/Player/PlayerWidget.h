// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/BaseWeapon.h"
#include "PlayerWidget.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool TryToGetCurrentAmmoData(FAmmoData& AmmoData)const;
	
};
