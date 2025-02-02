// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "GameModes/MainGameMode.h"
#include "UI/Player/PlayerHUD.h"



void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()) {
		const auto GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode) {
			GameMode->OnGameStateChanged.AddUObject(this, &ABaseCharacterController::OnGameStateChanged);
		}
	}
}

void ABaseCharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	check(aPawn);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	Tags.Add(FName("Player_Controller"));

	const auto HPComponent = BaseCharacter->GetComponentByClass<UHealthComponent>();
	check(HPComponent);
	HPComponent->OnDeathDelegate.AddUObject(this, &ABaseCharacterController::OnDeathCallback);

	GetHUD<APlayerHUD>()->CloseDeathWidget();
}

void ABaseCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacterController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacterController::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacterController::Jump);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABaseCharacterController::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABaseCharacterController::StopFire);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABaseCharacterController::Reload);

		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ABaseCharacterController::SwitchWeapon);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABaseCharacterController::GamePause);

		EnhancedInputComponent->BindAction(OpenKdMenuAction, ETriggerEvent::Started, this, &ABaseCharacterController::OpenKdMenu);
		EnhancedInputComponent->BindAction(OpenKdMenuAction, ETriggerEvent::Completed, this, &ABaseCharacterController::CloseKdMenu);
	}
}

void ABaseCharacterController::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	
	if (GetPawn()) {
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Movement.Y);
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Movement.X);
	}
}

void ABaseCharacterController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (GetPawn()) {
		GetPawn()->AddControllerYawInput(LookVector.X);
		GetPawn()->AddControllerPitchInput(-LookVector.Y);

		GetPawn<ABaseCharacter>()->RotateHands(-LookVector.Y);
	}
}

void ABaseCharacterController::Jump(const FInputActionValue& Value)
{
	if (GetPawn()) {
		BaseCharacter->Jump();
	}
}

void ABaseCharacterController::StartFire(const FInputActionValue& Value)
{
	if(!GetPawn()) return;
	
	if (auto WeaponComponent = GetPawn()->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->StartFire();
	}
}

void ABaseCharacterController::StopFire(const FInputActionValue& Value)
{
	if(!GetPawn()) return;

	if (auto WeaponComponent = GetPawn()->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->StopFire();
	}
}

void ABaseCharacterController::Reload(const FInputActionValue& Value)
{
	if(!GetPawn()) return;
	
	if (auto WeaponComponent = GetPawn()->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->Reload();
	}
}

void ABaseCharacterController::SwitchWeapon(const FInputActionValue& Value)
{
	if(!GetPawn()) return;
	
	if (auto WeaponComponent = GetPawn()->FindComponentByClass<UWeaponComponent>()) {
		WeaponComponent->SwitchWeapon();
	}
}

void ABaseCharacterController::GamePause(const FInputActionValue& Value)
{
	if (!GetWorld()) {
		return;
	}

	auto GameMode = GetWorld()->GetAuthGameMode();

	if (!GameMode) {
		return;
	}

	GameMode->SetPause(this);
}

void ABaseCharacterController::OnGameStateChanged(EGameState NewState)
{
	if (NewState == EGameState::InGame) {
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else {
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ABaseCharacterController::OpenKdMenu(const FInputActionValue& InputActionValue)
{
	if(const auto PlayerHud = GetHUD<APlayerHUD>())
		PlayerHud->PopupKdMenu();
}

void ABaseCharacterController::CloseKdMenu(const FInputActionValue& InputActionValue)
{
	if(const auto PlayerHud = GetHUD<APlayerHUD>())
		PlayerHud->CloseKdMenu();
}

void ABaseCharacterController::OnDeathCallback(AController* Damaged, AController* Causer)
{
	const auto GameModeDM = GetWorld()->GetAuthGameMode<AMainGameMode>();
	check(GameModeDM && GetPawn());
	
	GetPawn()->SetLifeSpan(GameModeDM->GetRespawnTime());
	UnPossess();
	GetHUD<APlayerHUD>()->PopupDeathWidget();

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, GameModeDM->GetRespawnTime(), false);
}

ETeamAttitude::Type ABaseCharacterController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return IGenericTeamAgentInterface::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}
	return IGenericTeamAgentInterface::GetTeamAttitudeTowards(Other);
}

float ABaseCharacterController::GetRemainingRespawnTime() const
{
	const auto RemainingTime =  GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
	return (RemainingTime == -1.f) ? 0.f : RemainingTime;
}

