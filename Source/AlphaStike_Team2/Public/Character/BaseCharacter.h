// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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
private:
	void UpdateCameraOffset();

protected:
	virtual void BeginPlay() override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Robot Settings")
	float HandsRotationAngle{50.f};

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<class UWidgetComponent> HealthBarWidgetComponent{ nullptr };

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UHealthBarWidget> HealthBarWidgetClass;
	
public:	
	UPROPERTY(EditAnywhere, Category = "Game Settings")
	bool CameraWalkShake{true};

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RotateBody();
	void RotateHands(float LookAtTarget);
	void InitializeHealthBar();
	bool HandsRotationInRange(const float& LookAtTarget);
};
