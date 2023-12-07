// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USoundCue;

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

USTRUCT(BlueprintType)
struct FWeaponData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USkeletalMesh* Weapon_Skeletal_Mesh{ nullptr };
};


UCLASS()
class ALPHASTIKE_TEAM2_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	
public:	
	ABaseWeapon();

	void InitForAI();

	virtual void StartFire(){}
	virtual void StopFire(){}

	void Reload();
	inline bool CanReload()const {
		return CurrentAmmoData.BulletsNum < DefaultAmmoData.BulletsNum && CurrentAmmoData.ClipNum >0;
	}

	void AddClips(int32 Delta);
	
	inline FAmmoData GetAmmoData()const {
		return CurrentAmmoData;
	}

	inline FWeaponData GetWeaponData() const{
		return WeaponData;
	}

	FOnClipEmpty OnClipEmpty;

protected:

	virtual void BeginPlay() override;

	void DecreaseAmmo();

	inline bool IsAmmoEmpty()const {
		return !CurrentAmmoData.Infinite && CurrentAmmoData.BulletsNum == 0 && CurrentAmmoData.ClipNum == 0;
	}

	inline bool IsClipEmpty()const {
		return CurrentAmmoData.BulletsNum == 0;
	}

	bool CalculateTrace(FVector& EndPoint)const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ammo")
	FAmmoData DefaultAmmoData;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ammo|AI")
	FAmmoData AIAmmoData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float ShotDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmountOfDamage = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ammo|AI")
	float AIAmountOfDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName LeftHandMuzzleSocketName = "LeftHandMuzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FName RightHandMuzzleSocketName = "RightHandMuzzle";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Spread = 2.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon|AI")
	float AISpread = 2.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Sound")
	USoundCue* FireSound;

	int32 HandIndex = 0;

private:

	FAmmoData CurrentAmmoData;
};
