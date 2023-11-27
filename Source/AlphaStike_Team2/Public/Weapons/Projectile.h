// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class ALPHASTIKE_TEAM2_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	void SetupDirection(const FVector& Direction);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Component")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket")
	float Damage = 30;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket")
	float DamageRadius = 150.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rocket")
	bool DoFullDamage = false;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector Direction;
};
