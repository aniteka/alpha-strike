// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AutoRifle.h"
#include "Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"

AAutoRifle::AAutoRifle() {
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(WeaponMesh);
}

void AAutoRifle::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
}

void AAutoRifle::StartFire()
{
	GetWorldTimerManager().SetTimer(StartFireTimerHandle, this, &AAutoRifle::Shot, 0.1f, true, 0.0f);
}

void AAutoRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(StartFireTimerHandle);
}

void AAutoRifle::Shot()
{
	if (IsAmmoEmpty()) {
		return;
	}

	FVector StartPoint;
	FVector EndPoint;

	if (!CalculateTrace(StartPoint, EndPoint)) {
		return;
	}

	FHitResult HitResult;

	DrawDebugLine(GetWorld(), WeaponMesh->GetSocketLocation(MuzzleSocketName), EndPoint, FColor::Red, false, 3.f, 0, 3.f);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50.f, 24, FColor::Red, false, 3.f, 0, 3.f);

	if (HitResult.bBlockingHit) {
		const auto OtherPlayer = HitResult.GetActor();

		if (!OtherPlayer) {
			return;
		}

		const auto Player = Cast<ABaseCharacter>(GetOwner());

		if (!Player) {
			return;
		}

		OtherPlayer->TakeDamage(AmountOfDamage, FDamageEvent{}, Player->GetController(), this);

	}

	DecreaseAmmo();
}
