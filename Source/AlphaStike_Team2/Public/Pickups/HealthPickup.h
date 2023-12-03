// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickupActor.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALPHASTIKE_TEAM2_API AHealthPickup : public ABasePickupActor
{
	GENERATED_BODY()
protected:
	virtual void Pickup() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
	float HealAmount{50.f};
};
