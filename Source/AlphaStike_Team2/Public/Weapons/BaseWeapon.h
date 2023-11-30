// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmpty);

USTRUCT(BlueprintType)
struct FAmmoData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ammo")
	bool Infinite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (EditCondition = "!Infinite"))
	int32 ClipNum = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BulletsNum = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* WeaponIcon;
};

UCLASS()
class ALPHASTIKE_TEAM2_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	
public:	
	ABaseWeapon();

	virtual void StartFire(){}
	virtual void StopFire(){}

	void Reload();
	inline bool CanReload()const {
		return CurrentAmmoData.BulletsNum < DefaultAmmoData.BulletsNum && CurrentAmmoData.ClipNum >0;
	}

	inline FAmmoData GetAmmoData()const {
		return CurrentAmmoData;
	}

	FOnClipEmpty OnClipEmpty;

protected:

	virtual void BeginPlay() override;

	void DecreaseAmmo();

	inline bool IsAmmoEmpty()const {
		return !CurrentAmmoData.Infinite && CurrentAmmoData.BulletsNum == 0 && CurrentAmmoData.ClipNum == 0;
	}

	bool CalculateTrace(FVector& EndPoint)const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ammo")
	FAmmoData DefaultAmmoData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float ShotDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmountOfDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName LeftHandMuzzleSocketName = "LeftHandMuzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName RightHandMuzzleSocketName = "RightHandMuzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Spread = 2.f;

	int32 HandIndex = 0;
private:

	FAmmoData CurrentAmmoData;

	
};
