// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RobotHandWeapon.h"
#include "Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"


void ARobotHandWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARobotHandWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(StartFireTimerHandle, this, &ARobotHandWeapon::Shot, ShotRate, true,0.0f);
}


void ARobotHandWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(StartFireTimerHandle);
}

void ARobotHandWeapon::Shot()
{
	if (IsAmmoEmpty()) {
		return;
	}
	const auto Player = Cast<ABaseCharacter>(GetOwner());
	if (!Player) {
		return;
	}

	FVector StartPoint;
	FVector EndPoint;

	if (!CalculateTrace(StartPoint, EndPoint)) {
		return;
	}

	const auto LeftHandMuzzle = Player->GetLeftHandMesh()->GetSocketLocation(LeftHandMuzzleSocketName);
	const auto RightHandMuzzle = Player->GetRightHandMesh()->GetSocketLocation(RightHandMuzzleSocketName);

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(GetWorld(), HandIndex == 0 ? LeftHandMuzzle : RightHandMuzzle, EndPoint, FColor::Red, false, 3.f, 0, 3.f);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50.f, 24, FColor::Red, false, 3.f, 0, 3.f);

	HandIndex = (HandIndex + 1) % 2;

	DecreaseAmmo();

	if (HitResult.bBlockingHit) {
		const auto OtherPlayer = HitResult.GetActor();

		if (!OtherPlayer) {
			return;
		}

		OtherPlayer->TakeDamage(AmountOfDamage, FDamageEvent{}, Player->GetController(), this);

	}
}
