// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;

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

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

private:
	TArray<ABaseWeapon*> Weapons;

	ABaseWeapon* CurrentWeapon = nullptr;

	void CreateWeapon();
	bool CanReload()const;
};
