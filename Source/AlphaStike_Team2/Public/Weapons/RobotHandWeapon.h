// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RobotHandWeapon.generated.h"

class UNiagaraSystem;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* BeamTraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString BeamEndName = "BeamEnd";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
	bool bDrawDebugTracers = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* HitSound;
	
	
	void SpawnFX();

private:

	FTimerHandle StartFireTimerHandle;

	UAudioComponent* FireAudioComponent;

	void Shot();
	void ActiveFX(bool IsActive);
	void SetupTraceEffect(const FVector& StartPoint, const FVector& EndPoint);
	void SpawnShotDecal(const FHitResult& HitResult);

};
