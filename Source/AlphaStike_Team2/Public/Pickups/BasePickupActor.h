// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickupActor.generated.h"

class UStaticMeshComponent;
class URotatingMovementComponent;
class USphereComponent;

UCLASS()
class ALPHASTIKE_TEAM2_API ABasePickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickupActor();

	UFUNCTION()
	void OnBeginOverlapComponentEvent(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);

	void SetAvailable();
	void SetUnavailable();
	bool IsAvailable() const { return bIsAvailable; }
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pickup Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Components")
	TObjectPtr<URotatingMovementComponent> RotatingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Components")
	TObjectPtr<USphereComponent> ColliderComponent;	

	UPROPERTY(EditAnywhere, Category = "Options")
	float RespawnKd = 60.f;
	
	TObjectPtr<ABaseCharacter> Character;

	virtual void Pickup();

private:
	bool bIsAvailable = true;
};

