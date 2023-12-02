

#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::StartFire()
{
	if (CurrentWeapon && CanFire()) {
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

		const auto Player = Cast<ABaseCharacter>(GetOwner());
		if (!Player) {
			return;
		}

		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(ReloadSound, Player->GetMesh(), BagSocketName);
		IsReloadSoundPlaying = true;

		if (!AudioComponent) {
			return;
		}

		AudioComponent->OnAudioFinishedNative.AddLambda([&](UAudioComponent* AudioComponent) {
			IsReloadSoundPlaying = false;
			CurrentWeapon->Reload();
		});

	}
}

void UWeaponComponent::SwitchWeapon()
{
	if (CasSwitchWeapon()) {
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
		CurWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), BagSocketName);

		Weapons.Add(CurWeapon);
	}
}

void UWeaponComponent::TakeWeapon()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}

	CurrentWeapon = Weapons[WeaponIndex];
}


