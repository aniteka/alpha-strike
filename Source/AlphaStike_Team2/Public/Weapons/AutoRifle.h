// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "AutoRifle.generated.h"


UCLASS()
class ALPHASTIKE_TEAM2_API AAutoRifle : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AAutoRifle();

protected:

	virtual void BeginPlay()override;

	virtual void StartFire()override;
	virtual void StopFire()override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* WeaponMesh;


private:

	FTimerHandle StartFireTimerHandle;
	
	void Shot();

};
