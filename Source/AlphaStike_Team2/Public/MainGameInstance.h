// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlphaStrikeTypes.h"
#include "MainGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FLevelData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Level")
	UTexture2D* LevelImage;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
	FName LoadLevelName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Level")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
	FName LevelDescription = NAME_None;
};


UCLASS()
class ALPHASTIKE_TEAM2_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ETeamType GetPlayerTeamType() const { return PlayerTeamType; }
	void SetPlayerTeamType(ETeamType NewPlayerTeamType) { PlayerTeamType = NewPlayerTeamType; }

	TArray<FLevelData> GetLevelsData() {
		return Levels;
	}

protected:
	ETeamType PlayerTeamType = ETeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelData")
	TArray<FLevelData> Levels;
};
