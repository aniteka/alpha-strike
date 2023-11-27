// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Rifle.generated.h"

class AProjectile;

UCLASS()
class ALPHASTIKE_TEAM2_API ARifle : public ABaseWeapon
{
	GENERATED_BODY()


protected:
	virtual void StartFire()override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

private:

	void Shot();
};
