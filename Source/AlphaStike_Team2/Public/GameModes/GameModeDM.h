// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlphaStrikeTypes.h"
#include "GameFramework/GameMode.h"
#include "GameModeDM.generated.h"

class AAIRoute;
class ADEPRECATED_AIDeathMatchTeamManager;
class AAIController;
class ABaseCharacter;
class ATargetPoint;

USTRUCT(BlueprintType)
struct FBotSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bSpawn = true;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ATargetPoint> SpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseCharacter> BotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAIController> BotController;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FBotSpawnInfo> Team;

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<AAIRoute>> Routes;
};

UENUM(BlueprintType)
enum class EGameState : uint8 {
	StartGame = 0,
	InGame,
	Pause
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState);

UCLASS()
class ALPHASTIKE_TEAM2_API AGameModeDM : public AGameMode
{
	GENERATED_BODY()

public:
	AGameModeDM();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

	ETeamType GetPlayerTeamType() const { return PlayerTeamType; }
	void SetPlayerTeamType(ETeamType NewPlayerTeamType) { PlayerTeamType = NewPlayerTeamType; }

	int32 GetPlayerSpawnIndex() const { return PlayerSpawnIndex; }

	TSoftObjectPtr<AAIRoute> GetRouteForTeam(ETeamType Type);

	FOnGameStateChanged OnGameStateChanged;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause())override;
	virtual bool ClearPause();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Deathmatch|Spawn")
	TMap<ETeamType, FTeamInfo> TeamInfos;

	UPROPERTY(EditAnywhere, Category = "Deathmatch|Player")
	ETeamType PlayerTeamType = ETeamType::Blue;

	// -1 == Random
	UPROPERTY(EditAnywhere, Category = "Deathmatch|Player", meta = (ClampMin = 0, UIMax = 10))
	int32 PlayerSpawnIndex = -1;

private:
	void InitPlayerSpawnIndex();
	void InitPlayerTeamType();
	
	AController* SpawnPlayerInsteadOfBot();
	
	void SpawnAllTeams();
	void SpawnTeam(const FTeamInfo& TeamInfo, ETeamType Type);
	ACharacter* SpawnBotByInfo(const FBotSpawnInfo& SpawnInfo) const;
	void SetNewGameState(EGameState NewState);

	EGameState State;
};
