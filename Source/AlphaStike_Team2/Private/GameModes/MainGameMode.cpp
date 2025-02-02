// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/MainGameMode.h"

#include "AIController.h"
#include "MainGameInstance.h"
#include "Ai/AIDeathMatchCharacterController.h"
#include "Ai/DeathMatch/DEPRECATED_AIDeathMatchTeamManager.h"
#include "Character/BaseCharacter.h"
#include "Engine/TargetPoint.h"
#include "GameStates/MainGameState.h"
#include "UI/Player/PlayerHUD.h"

bool FBotSpawnInfo::IsValid() const
{
	return SpawnPoint.IsValid() && BotClass.Get() && BotController.Get();
}



AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
	GameStateClass = AMainGameState::StaticClass();
}

void AMainGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	FGenericTeamId::SetAttitudeSolver(MainTeamAttitudeSolver);

	SetDefaultPawn();
	InitMatchCountdown();
	
	InitPlayerTeamType();
	InitPlayerSpawnIndex();

	RespawnAndInitPlayer();

	SpawnAllTeams();
}


TSoftObjectPtr<AAIRoute> AMainGameMode::GetRouteForTeam(ETeamType Type)
{
	if(Type == ETeamType::None || !TeamInfos.Find(Type) || TeamInfos[Type].Routes.Num() == 0)
		return nullptr;
	return TeamInfos[Type].Routes[FMath::RandRange(0, TeamInfos[Type].Routes.Num() - 1)];
}


UMaterialInterface* AMainGameMode::GetMaterialForTeam(ETeamType Type) const
{
	return TeamInfos[Type].TeamMaterial;
}

AController* AMainGameMode::RespawnAndInitPlayer()
{
	check(TeamInfos[PlayerTeamType].Team.IsValidIndex(GetPlayerSpawnIndex()));

	auto& SpawnInfo = TeamInfos[PlayerTeamType].Team[GetPlayerSpawnIndex()];
	check(SpawnInfo.IsValid());
	
	RestartPlayerAtTransform(GetWorld()->GetFirstPlayerController(), SpawnInfo.SpawnPoint->GetTransform());
	SpawnInfo.bSpawn = false;
	
	const auto PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);
	if(const auto TeamAgent = Cast<IGenericTeamAgentInterface>(PlayerController))
		TeamAgent->SetGenericTeamId({static_cast<uint8>(GetPlayerTeamType())});
	InitTeamsVisualSignsForCharacter(PlayerController->GetCharacter(), GetMaterialForTeam(PlayerTeamType));
	
	return PlayerController;
}

ACharacter* AMainGameMode::RespawnAndInitBotByController(AAIDeathMatchCharacterController* Controller)
{
	const auto SpawnInfo = Controller->GetSpawnInfo();
	if(!SpawnInfo.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnInfo is not valid"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);

	Controller->Possess(Bot);

	InitTeamsVisualSignsForCharacter(
		Bot, GetMaterialForTeam(static_cast<ETeamType>(Controller->GetGenericTeamId().GetId())));
	
	return Bot;
}

float AMainGameMode::GetRemainingMatchCountdown() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(MatchCountdown);
}

void AMainGameMode::InitMatchCountdown()
{
	GetWorld()->GetTimerManager()
		.SetTimer(MatchCountdown, this, &AMainGameMode::TryEndMatch, DefaultMatchCountdown);
}

void AMainGameMode::TryEndMatch()
{
	const auto PlayerController = GetWorld()->GetFirstPlayerController();
	AGameMode::SetPause(PlayerController);
	PlayerController->GetHUD<APlayerHUD>()->PopupEndgameMenu();
}

void AMainGameMode::InitPlayerSpawnIndex()
{
	if(PlayerSpawnIndex == -1)
		PlayerSpawnIndex = FMath::RandRange(0, TeamInfos[PlayerTeamType].Team.Num() - 1);
}

void AMainGameMode::InitPlayerTeamType()
{
	const auto MainGameInstance = GetGameInstance<UMainGameInstance>();
	check(MainGameInstance);
	if(MainGameInstance->GetPlayerTeamType() != ETeamType::None)
		SetPlayerTeamType(MainGameInstance->GetPlayerTeamType());
	if(!TeamInfos.Contains(PlayerTeamType))
		UE_LOG(LogTemp, Error, TEXT("TeamInfos.Contains(PlayerTeamType) == false"));
}

void AMainGameMode::InitTeamsVisualSignsForCharacter(ACharacter* Character, UMaterialInterface* TeamMaterial) const
{
	if(const auto BaseCharacter = Cast<ABaseCharacter>(Character))
		BaseCharacter->InitTeamsVisualSigns(TeamMaterial);
}

void AMainGameMode::SpawnAllTeams()
{
	for (const auto& TeamInfo : TeamInfos)
		SpawnTeam(TeamInfo.Value, TeamInfo.Key);
}

void AMainGameMode::SpawnTeam(const FTeamInfo& TeamInfo, ETeamType Type)
{
	for (const auto& BotInfo : TeamInfo.Team)
		if(BotInfo.bSpawn)
		{
			const auto Bot = SpawnAndInitBotByInfo(BotInfo, Type);
			check(Bot);
		}
}

ACharacter* AMainGameMode::SpawnAndInitBotByInfo(const FBotSpawnInfo& SpawnInfo, ETeamType TeamType) const
{
	if(!SpawnInfo.IsValid())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);
	if(!Bot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn bot"));
		return nullptr;
	}
	
	if(SpawnInfo.BotController)
	{
		Bot->AIControllerClass = SpawnInfo.BotController.Get();
		if(const auto BotController = Bot->GetController())
		{
			BotController->UnPossess();
			BotController->Destroy();
		}
		Bot->SpawnDefaultController();
	}

	if(const auto DMCharController = Bot->GetController<AAIDeathMatchCharacterController>())
		DMCharController->SetSpawnInfo(SpawnInfo);

	if(const auto TeamAgent = Bot->GetController<IGenericTeamAgentInterface>())
		TeamAgent->SetGenericTeamId({static_cast<uint8>(TeamType)});
	
	InitTeamsVisualSignsForCharacter(Bot, GetMaterialForTeam(TeamType));
	
	return Bot;
}

void AMainGameMode::SetDefaultPawn()
{
	auto GameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return;
	}

	DefaultPawnClass = GameInstance->GetBaseCharacter();
}


