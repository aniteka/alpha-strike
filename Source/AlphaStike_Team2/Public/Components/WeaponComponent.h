// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Weapons/RobotHandWeapon.h"
#include "InputActionValue.h"
#include "WeaponComponent.generated.h"

class USoundCue;
class UWeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartWeaponReloading, UWeaponComponent*, WeaponComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndWeaponReloading, UWeaponComponent*, WeaponComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHASTIKE_TEAM2_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	void StartFire();
	void StopFire();
	void Reload();
	void SwitchWeapon();
	bool TryToGetCurrentAmmoData(FAmmoData& AmmoData)const;

	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName BagSocketName = "BagSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ReloadSound;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStartWeaponReloading OnStartWeaponReloading;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnEndWeaponReloading OnEndWeaponReloading;
	
private:
	TArray<ABaseWeapon*> Weapons;

	ABaseWeapon* CurrentWeapon = nullptr;

	int32 WeaponIndex = 0;

	bool IsReloadSoundPlaying = false;

	void CreateWeapon();
	inline bool CanReload()const{ return CurrentWeapon->CanReload() && !IsReloadSoundPlaying; }
	inline bool CanFire()const { return CurrentWeapon && !IsReloadSoundPlaying; }
	inline bool CasSwitchWeapon()const { return Weapons.Num() > 0 && !IsReloadSoundPlaying; }

	void TakeWeapon();
};
