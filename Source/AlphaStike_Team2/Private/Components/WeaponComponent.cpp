

#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Character/BaseCharacter.h"

UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}

void UWeaponComponent::StartFire()
{
	UE_LOG(LogTemp, Display, TEXT("StartFire"));
	if (CurrentWeapon) {
		CurrentWeapon->StartFire();
	}
}

void UWeaponComponent::StopFire()
{
	UE_LOG(LogTemp, Display, TEXT("StopFire"));
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
}

void UWeaponComponent::Reload()
{
	UE_LOG(LogTemp, Display, TEXT("Reload"));
	if (CurrentWeapon && CanReload()) {
		CurrentWeapon->Reload();
	}
}

void UWeaponComponent::SwitchWeapon()
{
	UE_LOG(LogTemp, Display, TEXT("SwitchWeapon"));
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponClasses.Num() > 0, TEXT("Need to add WeaponClass"));

	CreateWeapon();
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


	CurrentWeapon = Weapons[0];
}

bool UWeaponComponent::CanReload() const
{
	return CurrentWeapon->CanReload();
}


