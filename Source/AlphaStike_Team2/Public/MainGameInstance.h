// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlphaStrikeTypes.h"
#include "MainGameInstance.generated.h"

UCLASS()
class ALPHASTIKE_TEAM2_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ETeamType GetPlayerTeamType() const { return PlayerTeamType; }
	void SetPlayerTeamType(ETeamType NewPlayerTeamType) { PlayerTeamType = NewPlayerTeamType; }

protected:
	ETeamType PlayerTeamType = ETeamType::None;
};
