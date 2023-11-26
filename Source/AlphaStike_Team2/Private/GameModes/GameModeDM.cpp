// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameModeDM.h"

#include "AIController.h"
#include "MainGameInstance.h"
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
	SpawnPlayerInsteadOfBot();
	SpawnTeams();
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

void AGameModeDM::SpawnPlayerInsteadOfBot()
{
	if(!TeamInfos[PlayerTeamType].Team.IsValidIndex(GetPlayerSpawnIndex()))
	{
		UE_LOG(LogTemp, Error, TEXT("TeamInfos[PlayerTeamType].Team.IsValidIndex(SpawnIndex) == nullptr"));
		return;
	}
	SpawnPlayerByInfo(TeamInfos[PlayerTeamType].Team[GetPlayerSpawnIndex()]);
	TeamInfos[PlayerTeamType].Team[GetPlayerSpawnIndex()].bSpawn = false;
}

void AGameModeDM::SpawnTeams()
{
	for (const auto& TeamInfo : TeamInfos)
		SpawnTeam(TeamInfo.Value, TeamInfo.Key);
}

void AGameModeDM::SpawnTeam(const FTeamInfo& TeamInfo, ETeamType Type)
{
	for (const auto& BotInfo : TeamInfo.Team)
		if(BotInfo.bSpawn)
			SpawnBotByInfo(BotInfo);
}

void AGameModeDM::SpawnPlayerByInfo(const FBotSpawnInfo& SpawnInfo)
{
	if(!SpawnInfo.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnInfo is not valid"));
		return;
	}
	
	DefaultPawnClass = SpawnInfo.BotClass.Get();
	RestartPlayerAtTransform(GetWorld()->GetFirstPlayerController(), SpawnInfo.SpawnPoint->GetTransform());
}

void AGameModeDM::SpawnBotByInfo(const FBotSpawnInfo& SpawnInfo)
{
	if(!SpawnInfo.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnInfo is not valid"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);

	Bot->AIControllerClass = SpawnInfo.BotController.Get();
	Bot->SpawnDefaultController();
}
