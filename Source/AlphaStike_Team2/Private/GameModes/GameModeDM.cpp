// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/GameModeDM.h"

#include "AIController.h"
#include "MainGameInstance.h"
#include "Ai/DeathMatch/DEPRECATED_AIDeathMatchTeamManager.h"
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

	const auto PlayerController = SpawnPlayerInsteadOfBot();
	if(const auto TeamAgent = Cast<IGenericTeamAgentInterface>(PlayerController))
		TeamAgent->SetGenericTeamId({static_cast<uint8>(GetPlayerTeamType())});
	
	SpawnAllTeams();
}

void AGameModeDM::StartPlay()
{
	Super::StartPlay();
	SetNewGameState(EGameState::InGame);
}

TSoftObjectPtr<AAIRoute> AGameModeDM::GetRouteForTeam(ETeamType Type)
{
	if(Type == ETeamType::None || !TeamInfos.Find(Type) || TeamInfos[Type].Routes.Num() == 0)
		return nullptr;
	return TeamInfos[Type].Routes[FMath::RandRange(0, TeamInfos[Type].Routes.Num() - 1)];
}

bool AGameModeDM::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	SetNewGameState(EGameState::Pause);
	return Super::SetPause(PC,CanUnpauseDelegate);
}

bool AGameModeDM::ClearPause()
{
	SetNewGameState(EGameState::InGame);
	return Super::ClearPause();
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

AController* AGameModeDM::SpawnPlayerInsteadOfBot()
{
	check(TeamInfos[PlayerTeamType].Team.IsValidIndex(GetPlayerSpawnIndex()));

	auto& SpawnInfo = TeamInfos[PlayerTeamType].Team[GetPlayerSpawnIndex()];
	check(SpawnInfo.IsValid());
	
	RestartPlayerAtTransform(GetWorld()->GetFirstPlayerController(), SpawnInfo.SpawnPoint->GetTransform());
	SpawnInfo.bSpawn = false;

	return GetWorld()->GetFirstPlayerController();
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
			if(const auto TeamAgent = Bot->GetController<IGenericTeamAgentInterface>())
				TeamAgent->SetGenericTeamId({static_cast<uint8>(Type)});
		}
}

ACharacter* AGameModeDM::SpawnBotByInfo(const FBotSpawnInfo& SpawnInfo) const
{
	if(!SpawnInfo.IsValid())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);

	Bot->AIControllerClass = SpawnInfo.BotController.Get();
	Bot->SpawnDefaultController();
	return Bot;
}

void AGameModeDM::SetNewGameState(EGameState NewState)
{
	if (State == NewState) {
		return;
	}

	State = NewState;
	OnGameStateChanged.Broadcast(NewState);
}
