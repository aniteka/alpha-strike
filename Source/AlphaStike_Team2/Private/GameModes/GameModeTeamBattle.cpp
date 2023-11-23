// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameModeTeamBattle.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterController.h"
#include "AIController.h"

AGameModeTeamBattle::AGameModeTeamBattle()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ABaseCharacterController::StaticClass();
}

void AGameModeTeamBattle::StartPlay()
{
	Super::StartPlay();

	CurrentRound = 1;
}

void AGameModeTeamBattle::SpawnPlayers()
{
	if (!GetWorld()) {
		return;
	}

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i) {
		//GetWorld()->SpawnActor<AAIController>();
	}
}
