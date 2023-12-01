// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/GameModeDM.h"

#include "AIController.h"
#include "MainGameInstance.h"
#include "Ai/DeathMatch/AIDeathMatchCharacterController.h"
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
	InitTeamsVisualSignsForCharacter(PlayerController->GetCharacter(), GetMaterialForTeam(PlayerTeamType));
	
	SpawnAllTeams();
}

TSoftObjectPtr<AAIRoute> AGameModeDM::GetRouteForTeam(ETeamType Type)
{
	if(Type == ETeamType::None || !TeamInfos.Find(Type) || TeamInfos[Type].Routes.Num() == 0)
		return nullptr;
	return TeamInfos[Type].Routes[FMath::RandRange(0, TeamInfos[Type].Routes.Num() - 1)];
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
	
	return Bot;
}

ACharacter* AGameModeDM::SpawnBotByInfoWithController(const FBotSpawnInfo& SpawnInfo, AController* Controller)
{
	if(!SpawnInfo.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnInfo is not valid"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const auto Bot = GetWorld()->SpawnActor<ACharacter>(SpawnInfo.BotClass, SpawnInfo.SpawnPoint->GetTransform(), SpawnParams);

	Controller->Possess(Bot);

	if(const auto DMCharController = Bot->GetController<AAIDeathMatchCharacterController>())
		DMCharController->SetSpawnInfo(SpawnInfo);

	return Bot;
}

UMaterial* AGameModeDM::GetMaterialForTeam(ETeamType Type)
{
	return TeamInfos[Type].TeamMaterial;
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

void AGameModeDM::InitTeamsVisualSignsForCharacter(ACharacter* Character, UMaterial* TeamMaterial)
{
	if(const auto BaseCharacter = Cast<ABaseCharacter>(Character))
		BaseCharacter->InitTeamsVisualSigns(TeamMaterial);
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
			InitTeamsVisualSignsForCharacter(Bot, GetMaterialForTeam(Type));
		}
}
