// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTeamBattle.generated.h"

USTRUCT(BlueprintType)
struct FGameData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GameData",meta = (ClampMin = "1", ClampMax = "20"))
	int32 RoundsNum = 5;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GameData",meta = (ClampMin = "1", ClampMax = "300"))
	int32 RoundTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", meta = (ClampMin = "2", ClampMax = "30"))
	int32 PlayersNum = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	TArray<FLinearColor> TeamColors;
};

UCLASS()
class ALPHASTIKE_TEAM2_API AGameModeTeamBattle : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeTeamBattle();

protected:

	virtual void StartPlay()override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	FGameData GameData;

	void SpawnPlayers();

private:
	int32 CurrentRound;
	
};
