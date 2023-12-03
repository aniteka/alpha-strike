// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UCameraComponent;
class USpringArmComponent;
class UWeaponComponent;
class UHealthComponent;

UCLASS()
class ALPHASTIKE_TEAM2_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABaseCharacter();

	inline const UStaticMeshComponent* GetRightHandMesh() const {
		return R_Hand;
	}

	inline const UStaticMeshComponent* GetLeftHandMesh() const {
		return L_Hand;
	}

protected:
	virtual void BeginPlay() override;

	void ShowHealthBarOnDamageTaken();
	void UpdateHealthBarVisibility();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* ArmComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWeaponComponent* WeaponComponent;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshBody{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> R_Hand{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> L_Hand{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Robot Settings")
	float HandsRotationAngle{50.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Settings")
	FVector DeathExplosionHeadImpulse = FVector::UpVector * 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Settings")
	UParticleSystem* ExplosionParticle = nullptr;

	
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<class UWidgetComponent> HealthBarWidgetComponent{ nullptr };

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	float TimeToShowHealthOnDamage{ 3.f };
	float LastTimeDamageTaken{ 0.f };

private:
	void UpdateCameraOffset();

	void OnDeathCallback(AController* Damaged, AController* Causer);
	
	bool TakenDamageRecently = false;
public:	
	UPROPERTY(EditAnywhere, Category = "Game Settings")
	bool CameraWalkShake{true};

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void InitTeamsVisualSigns(UMaterial* Material);

	void RotateBody();
	void RotateHands(float LookAtTarget);
	void InitializeHealthBar();
	bool HandsRotationInRange(const float& LookAtTarget);
};
