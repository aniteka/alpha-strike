// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Rifle.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API ARifle : public ABaseWeapon
{
	GENERATED_BODY()
public:
	ARifle();

protected:

	virtual void BeginPlay()override;

	virtual void StartFire()override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* WeaponMesh;

private:

	void Shot();
};
