

#include "Components/WeaponComponent.h"


UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}

void UWeaponComponent::StartFire()
{
	UE_LOG(LogTemp, Display, TEXT("StartFire"));
}

void UWeaponComponent::StopFire()
{
	UE_LOG(LogTemp, Display, TEXT("StopFire"));
}

void UWeaponComponent::Reload()
{
	UE_LOG(LogTemp, Display, TEXT("Reload"));
}

void UWeaponComponent::SwitchWeapon()
{
	UE_LOG(LogTemp, Display, TEXT("SwitchWeapon"));
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

}


