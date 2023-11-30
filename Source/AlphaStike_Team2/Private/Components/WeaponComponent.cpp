

#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Character/BaseCharacter.h"

UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}

void UWeaponComponent::StartFire()
{
	if (CurrentWeapon) {
		UE_LOG(LogTemp, Display, TEXT("StartFire"));
		CurrentWeapon->StartFire();
	}
}

void UWeaponComponent::StopFire()
{
	if (CurrentWeapon) {
		UE_LOG(LogTemp, Display, TEXT("StopFire"));
		CurrentWeapon->StopFire();
	}
}

void UWeaponComponent::Reload()
{
	if (CurrentWeapon && CanReload()) {
		UE_LOG(LogTemp, Display, TEXT("Reload"));
		CurrentWeapon->Reload();
	}
}

void UWeaponComponent::SwitchWeapon()
{
	if (Weapons.Num() > 1) {
		UE_LOG(LogTemp, Display, TEXT("SwitchWeapon"));
		WeaponIndex = (WeaponIndex + 1) % Weapons.Num();
		TakeWeapon();
	}
}

bool UWeaponComponent::TryToGetCurrentAmmoData(FAmmoData& AmmoData) const
{
	if (!CurrentWeapon) {
		return false;
	}
	
	AmmoData = CurrentWeapon->GetAmmoData();

	return true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponClasses.Num() > 0, TEXT("Need to add WeaponClass"));

	CreateWeapon();
	TakeWeapon();
}

void UWeaponComponent::CreateWeapon()
{
	if (!GetWorld()) {
		return;
	}

	auto Player = Cast<ABaseCharacter>(GetOwner());
	if (!Player) {
		return;
	}


	for (auto Weapon : WeaponClasses) {
		auto CurWeapon = GetWorld()->SpawnActor<ABaseWeapon>(Weapon);
		if (!CurWeapon) {
			continue;
		}

		CurWeapon->SetOwner(Player);
		CurWeapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::Reload);
		CurWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), WeaponSocketName);
	
		Weapons.Add(CurWeapon); 
	}	
}

bool UWeaponComponent::CanReload() const
{
	return CurrentWeapon->CanReload();
}

void UWeaponComponent::TakeWeapon()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}

	CurrentWeapon = Weapons[WeaponIndex];
}


