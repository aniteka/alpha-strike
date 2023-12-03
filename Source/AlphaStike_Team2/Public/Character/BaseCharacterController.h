// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameModes/GameModeDM.h"
#include "BaseCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABaseCharacter;

UCLASS()
class ALPHASTIKE_TEAM2_API ABaseCharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);	
	void Look(const FInputActionValue& Value);
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StartFire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void SwitchWeapon(const FInputActionValue& Value);
	void GamePause(const FInputActionValue& Value);
	void OnGameStateChanged(EGameState NewState);

public:

	/*
	 * Implementation of IGenericTeamAgentInterface
	 */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { GenericTeamId = TeamID; };
	virtual FGenericTeamId GetGenericTeamId() const override { return GenericTeamId; };
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SwitchWeaponAction;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;
	
private:
	ABaseCharacter* BaseCharacter;
	FGenericTeamId GenericTeamId;
};
