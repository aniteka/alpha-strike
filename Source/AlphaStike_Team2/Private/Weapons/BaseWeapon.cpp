// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Character/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWeapon::Reload()
{
	if (!CurrentAmmoData.Infinite) {
		if (CurrentAmmoData.ClipNum == 0) {
			return;
		}

		CurrentAmmoData.ClipNum--;
	}

	CurrentAmmoData.BulletsNum = DefaultAmmoData.BulletsNum;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoData = DefaultAmmoData;
	
}

void ABaseWeapon::DecreaseAmmo()
{
	CurrentAmmoData.BulletsNum--;
	UE_LOG(LogTemp, Display, TEXT("Bullets: %d/Clips: %d"), CurrentAmmoData.BulletsNum,CurrentAmmoData.ClipNum);

	if (CurrentAmmoData.BulletsNum == 0) {
		OnClipEmpty.Broadcast();
	}
}

bool ABaseWeapon::CalculateTrace(FVector& StartPoint, FVector& EndPoint) const
{

	const auto Player = Cast<ABaseCharacter>(GetOwner());
	if (!Player) {
		return false;
	}

	const auto Controller = Player->GetController();
	if (!Controller) {
		return false;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	StartPoint = CameraLocation;
	const auto Rad = FMath::DegreesToRadians(Spread);
	const auto Direction = FMath::VRandCone(CameraRotation.Vector(),Rad);
	EndPoint = StartPoint + Direction * ShotDistance;

	return true;
}

