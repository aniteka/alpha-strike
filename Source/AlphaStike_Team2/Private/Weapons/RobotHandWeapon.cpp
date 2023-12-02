// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RobotHandWeapon.h"
#include "Character/BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


void ARobotHandWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARobotHandWeapon::StartFire()
{
	SpawnFX();
	GetWorldTimerManager().SetTimer(StartFireTimerHandle, this, &ARobotHandWeapon::Shot, ShotRate, true,0.0f);
}


void ARobotHandWeapon::StopFire()
{
	ActiveFX(false);
	GetWorldTimerManager().ClearTimer(StartFireTimerHandle);
}

void ARobotHandWeapon::SpawnFX()
{
	if (!FireAudioComponent) {
		const auto Player = Cast<ABaseCharacter>(GetOwner());

		if (!Player) {
			return;
		}

		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, Player->GetLeftHandMesh(),LeftHandMuzzleSocketName);
	}

	ActiveFX(true);

}

void ARobotHandWeapon::Shot()
{
	if (IsClipEmpty()) {
		StopFire();
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

	const auto LeftHandMuzzle = Player->GetLeftHandMesh()->GetSocketLocation(LeftHandMuzzleSocketName);
	const auto RightHandMuzzle = Player->GetRightHandMesh()->GetSocketLocation(RightHandMuzzleSocketName);

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, HandIndex == 0 ? LeftHandMuzzle : RightHandMuzzle, EndPoint, ECollisionChannel::ECC_Visibility);
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

void ARobotHandWeapon::ActiveFX(bool IsActive)
{
	if (FireAudioComponent) {
		FireAudioComponent->SetPaused(!IsActive);
	}
}
