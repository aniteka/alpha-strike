// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/GameModeDM.h"

#include "AIController.h"
#include "MainGameInstance.h"
#include "Ai/DeathMatch/AIDeathMatchTeamManager.h"
#include "Character/BaseCharacter.h"
#include "Engine/TargetPoint.h"

bool FBotSpawnInfo::IsValid() const
{
	return SpawnPoint.IsValid() && BotClass.Get() && BotController.Get();
}



AGameModeDM::AGameModeDM()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
}

void AGameModeDM::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	InitPlayerTeamType();
	InitPlayerSpawnIndex();
	InitTeamManagers();

	const auto PlayerController = SpawnPlayerInsteadOfBot();
	SetTeamManagerForPlayerOrBot(PlayerController, GetPlayerTeamType());

	SpawnAllTeams();
}

void AGameModeDM::InitPlayerSpawnIndex()
{
	if(PlayerSpawnIndex == -1)
		PlayerSpawnIndex = FMath::RandRange(0, TeamInfos[PlayerTeamType].Team.Num() - 1);
}

void AGameModeDM::InitPlayerTeamType()
{
	const auto MainGameInstance = GetGameInstance<UMainGameInstance>();
	check(MainGameInstance);
	if(MainGameInstance->GetPlayerTeamType() != ETeamType::None)
		SetPlayerTeamType(MainGameInstance->GetPlayerTeamType());
	if(!TeamInfos.Contains(PlayerTeamType))
		UE_LOG(LogTemp, Error, TEXT("TeamInfos.Contains(PlayerTeamType) == false"));
}

void AGameModeDM::InitTeamManagers()
{
	for (const auto& TeamInfo : TeamInfos)
	{
		const auto TeamManager = TeamInfo.Value.DMTeamManager;
		if(!TeamManager)
		{
			UE_LOG(LogTemp, Error, TEXT("TeamManager is not setted"));
			return;
		}
		TeamManager->SetTeamType(TeamInfo.Key);
	}
}

AController* AGameModeDM::SpawnPlayerInsteadOfBot()
{
	check(TeamInfos[PlayerTeamType].Team.IsValidIndex(GetPlayerSpawnIndex()));

	auto& SpawnInfo = TeamInfos[PlayerTeamType].Team[GetPlayerSpawnIndex()];
	check(SpawnInfo.IsValid());
	
	RestartPlayerAtTransform(GetWorld()->GetFirstPlayerController(), SpawnInfo.SpawnPoint->GetTransform());
	SpawnInfo.bSpawn = false;

	return GetWorld()->GetFirstPlayerController();
}

void AGameModeDM::SetTeamManagerForPlayerOrBot(AController* Controller, ETeamType Type)
{
	const auto TeamManager = TeamInfos[Type].DMTeamManager.Get();
	if(!TeamManager)
	{
		UE_LOG(LogTemp, Error, TEXT("TeamManager is not setted"));
		return;
	}

	TeamManager->AddTeamMember(Controller);
}

void AGameModeDM::SpawnAllTeams()
{
	for (const auto& TeamInfo : TeamInfos)
		SpawnTeam(TeamInfo.Value, TeamInfo.Key);
}

void AGameModeDM::SpawnTeam(const FTeamInfo& TeamInfo, ETeamType Type)
{
	for (const auto& BotInfo : TeamInfo.Team)
		if(BotInfo.bSpawn)
		{
			const auto Bot = SpawnBotByInfo(BotInfo);
			SetTeamManagerForPlayerOrBot(Bot->GetController(), Type);
		}
}

ACharacter* AGameModeDM::SpawnBotByInfo(const FBotSpawnInfo& SpawnInfo) const
{
	if(!SpawnInfo.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnInfo is not valid"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);

	Bot->AIControllerClass = SpawnInfo.BotController.Get();
	Bot->SpawnDefaultController();
	return Bot;
}
