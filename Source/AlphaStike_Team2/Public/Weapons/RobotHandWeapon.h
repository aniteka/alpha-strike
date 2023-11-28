// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RobotHandWeapon.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API ARobotHandWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay()override;
	virtual void StartFire()override;
	virtual void StopFire()override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Shot")
	float ShotRate = 0.1f;

private:

	FTimerHandle StartFireTimerHandle;

	void Shot();

};
