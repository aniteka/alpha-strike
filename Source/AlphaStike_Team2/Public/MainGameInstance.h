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
	
	// Must be the same name as in GameModeData
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Level")
	FName LevelGameModeName = NAME_None;
};

USTRUCT(BlueprintType)
struct FGameModeData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "GameMode")
	FName GameModeName = NAME_None;
};

class ABaseCharacter;

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
	
	TArray<FGameModeData> GetGameModesData() {
		return GameModes;
	}
	
	TArray<ETeamType> GetTeamData() {
		return TeamTypes;
	}

	inline FName GetMenuLevelName()const {
		return MenuLevelName;
	}

	inline void SetBaseCharacter(TSubclassOf<ABaseCharacter> Character) {
		BaseCharacter = Character;
	}

	inline TSubclassOf<ABaseCharacter> GetBaseCharacter() {
		return BaseCharacter;
	}

protected:
	ETeamType PlayerTeamType = ETeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelData")
	TArray<FLevelData> Levels;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameModeData")
	TArray<FGameModeData> GameModes;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamData")
	TArray<ETeamType> TeamTypes;

	TSubclassOf<ABaseCharacter> BaseCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelData")
	FName MenuLevelName = "Menu";
};
