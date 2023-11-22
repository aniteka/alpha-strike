// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BaseCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABaseCharacter;

UCLASS()
class ALPHASTIKE_TEAM2_API ABaseCharacterController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

	void Move(const FInputActionValue& Value);	
	void Look(const FInputActionValue& Value);
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);


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


private:
	ABaseCharacter* BaseCharacter;

};
