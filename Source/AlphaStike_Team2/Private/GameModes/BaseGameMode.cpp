// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BaseGameMode.h"

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();
	SetNewGameState(EGameState::InGame);
}

bool ABaseGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	SetNewGameState(EGameState::Pause);
	return Super::SetPause(PC, CanUnpauseDelegate);
}

bool ABaseGameMode::ClearPause()
{
	SetNewGameState(EGameState::InGame);
	return Super::ClearPause();
}

void ABaseGameMode::SetNewGameState(EGameState NewState)
{
	if (State == NewState) {
		return;
	}

	State = NewState;
	OnGameStateChanged.Broadcast(NewState);
}