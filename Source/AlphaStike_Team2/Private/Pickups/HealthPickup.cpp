// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/HealthPickup.h"
#include "Components/HealthComponent.h"

void AHealthPickup::Pickup()
{
	Super::Pickup();

	UE_LOG(LogTemp, Warning, TEXT("------------HEAL------------"));
	Character->GetHealthComponent()->Heal(HealAmount);
}
