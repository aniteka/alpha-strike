// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerWidget.h"
#include "Character/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"

float UPlayerWidget::GetHealthPercent()
{
	const auto Player = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (!Player) {
		return 0.f;
	}

	const auto HealthComponent = Player->GetComponentByClass<UHealthComponent>();

	if (!HealthComponent) {
		return 0.f;
	}

	return HealthComponent->GetHealtPercent();
}

bool UPlayerWidget::TryToGetCurrentAmmoData(FAmmoData& AmmoData) const
{
	const auto Player = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (!Player) {
		return false;
	}

	const auto WeaponComponent = Player->GetComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) {
		return false;
	}

	return WeaponComponent->TryToGetCurrentAmmoData(AmmoData);
}
