// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/AmmoPickup.h"

#include "Components/WeaponComponent.h"
#include "Weapons/Rifle.h"

void AAmmoPickup::Pickup()
{
	Super::Pickup();

	const auto WeaponComponent = Character->GetComponentByClass<UWeaponComponent>();
	check(WeaponComponent);
	for (const auto Weapon : WeaponComponent->GetWeapons())
	{
		if(const auto Rifle = Cast<ARifle>(Weapon))
		{
			Rifle->AddClips(CountOfClipsToAdd);
			break;
		}
	}
}
