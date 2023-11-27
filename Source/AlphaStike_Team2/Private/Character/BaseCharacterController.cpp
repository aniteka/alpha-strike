// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/WeaponComponent.h"


void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();

	check(GetPawn());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	BaseCharacter = Cast<ABaseCharacter>(GetPawn());

}

void ABaseCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacterController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacterController::Look);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABaseCharacterController::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABaseCharacterController::StopCrouch);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacterController::Jump);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABaseCharacterController::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABaseCharacterController::StopFire);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABaseCharacterController::Reload);

		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ABaseCharacterController::SwitchWeapon);

	}
}

void ABaseCharacterController::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	
	if (BaseCharacter) {
		BaseCharacter->AddMovementInput(BaseCharacter->GetActorForwardVector(), Movement.Y);
		BaseCharacter->AddMovementInput(BaseCharacter->GetActorRightVector(), Movement.X);
	}
}

void ABaseCharacterController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (BaseCharacter) {
		BaseCharacter->AddControllerYawInput(LookVector.X);
		BaseCharacter->AddControllerPitchInput(-LookVector.Y);

		BaseCharacter->RotateHands(LookVector.Y);
	}
	
}

void ABaseCharacterController::StartCrouch(const FInputActionValue& Value)
{
	if (BaseCharacter) {
		BaseCharacter->Crouch();
	}
}

void ABaseCharacterController::StopCrouch(const FInputActionValue& Value)
{
	if (BaseCharacter) {
		BaseCharacter->UnCrouch();
	}
}

void ABaseCharacterController::Jump(const FInputActionValue& Value)
{
	if (BaseCharacter) {
		BaseCharacter->Jump();
	}
}

void ABaseCharacterController::StartFire(const FInputActionValue& Value)
{
	if (auto WeaponComponent = BaseCharacter->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->StartFire();
	}
}

void ABaseCharacterController::StopFire(const FInputActionValue& Value)
{
	if (auto WeaponComponent = BaseCharacter->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->StopFire();
	}
}

void ABaseCharacterController::Reload(const FInputActionValue& Value)
{
	if (auto WeaponComponent = BaseCharacter->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->Reload();
	}
}

void ABaseCharacterController::SwitchWeapon(const FInputActionValue& Value)
{
	if (auto WeaponComponent = BaseCharacter->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->SwitchWeapon();
	}
}

