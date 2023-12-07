// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickupActor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class ALPHASTIKE_TEAM2_API AAmmoPickup : public ABasePickupActor
{
	GENERATED_BODY()

public:
	virtual void Pickup() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Options")
	int32 CountOfClipsToAdd = 3;
};
