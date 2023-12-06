// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlphaStrikeTypes.h"
#include "GameModes/BaseGameMode.h"
#include "MainGameMode.generated.h"

class AAIDeathMatchCharacterController;
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

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TeamMaterial;
};


UCLASS()
class ALPHASTIKE_TEAM2_API AMainGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	AMainGameMode();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	ETeamType GetPlayerTeamType() const { return PlayerTeamType; }
	void SetPlayerTeamType(ETeamType NewPlayerTeamType) { PlayerTeamType = NewPlayerTeamType; }

	int32 GetPlayerSpawnIndex() const { return PlayerSpawnIndex; }

	TSoftObjectPtr<AAIRoute> GetRouteForTeam(ETeamType Type);

	FOnGameStateChanged OnGameStateChanged;

	UFUNCTION(BlueprintPure)
	UMaterialInterface* GetMaterialForTeam(ETeamType Type) const;

	float GetRespawnTime() const { return RespawnTime; }

	UFUNCTION(BlueprintCallable)
	AController* RespawnAndInitPlayer();

	UFUNCTION(BlueprintCallable)
	ACharacter* RespawnAndInitBotByController(AAIDeathMatchCharacterController* Controller);

	UFUNCTION(BlueprintPure, Category = "Deathmatch")
	float GetRemainingMatchCountdown() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deathmatch|Spawn")
	TMap<ETeamType, FTeamInfo> TeamInfos;

	UPROPERTY(EditAnywhere, Category = "Deathmatch|Player")
	ETeamType PlayerTeamType = ETeamType::Blue;

	// -1 == Random
	UPROPERTY(EditAnywhere, Category = "Deathmatch|Player", meta = (ClampMin = 0, UIMax = 10))
	int32 PlayerSpawnIndex = -1;

	UPROPERTY(EditAnywhere, Category = "Deathmatch")
	float RespawnTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "Deathmatch")
	float DefaultMatchCountdown = 4.f * 60.f;
	
private:
	void InitMatchCountdown();
	void TryEndMatch();
	
	void InitPlayerSpawnIndex();
	void InitPlayerTeamType();
	
	void InitTeamsVisualSignsForCharacter(ACharacter* Character, UMaterialInterface* TeamMaterial) const;

	void SpawnAllTeams();
	void SpawnTeam(const FTeamInfo& TeamInfo, ETeamType Type);
	ACharacter* SpawnAndInitBotByInfo(const FBotSpawnInfo& SpawnInfo, ETeamType TeamType) const;

	EGameState State;
	FTimerHandle MatchCountdown;
};
