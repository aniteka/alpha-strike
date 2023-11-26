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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName LeftHandMuzzleSocketName = "LeftHandMuzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName RightHandMuzzleSocketName = "RightHandMuzzle";

private:

	FTimerHandle StartFireTimerHandle;

	void Shot();

	int32 HandIndex = 0;

};
