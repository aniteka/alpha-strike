// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Rifle.h"
#include "Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Weapons/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void ARifle::StartFire()
{
	Shot();
}

void ARifle::Shot()
{
	if (IsAmmoEmpty()) {
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AmmoEmptySound, GetActorLocation());
		return;
	}

	const auto Player = Cast<ABaseCharacter>(GetOwner());
	if (!Player) {
		return;
	}

	FVector EndPoint;

	if (!CalculateTrace(EndPoint)) {
		return;
	}

	const auto LeftHandMuzzleLocation = Player->GetLeftHandMesh()->GetSocketLocation(LeftHandMuzzleSocketName);
	const auto RightHandMuzzleLocation = Player->GetRightHandMesh()->GetSocketLocation(RightHandMuzzleSocketName);

	FHitResult HitResult;

	auto Direction = (EndPoint - (HandIndex == 0 ? LeftHandMuzzleLocation: RightHandMuzzleLocation)).GetSafeNormal();

	FRotator BulletRotation = { Player->GetRightHandMesh()->GetSocketRotation(RightHandMuzzleSocketName)};

	FTransform Spawn(BulletRotation, (HandIndex == 0 ? LeftHandMuzzleLocation : RightHandMuzzleLocation));
	auto Rocket = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, Spawn);

	if (Rocket) {
		Rocket->SetOwner(GetOwner());
		Rocket->SetupDirection(Direction);
		Rocket->FinishSpawning(Spawn);
	}

	HandIndex = (HandIndex + 1) % 2;
	DecreaseAmmo();
}