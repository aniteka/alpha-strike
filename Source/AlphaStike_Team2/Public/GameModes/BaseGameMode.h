// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 {
	StartGame = 0,
	InGame,
	Pause
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState);

UCLASS()
class ALPHASTIKE_TEAM2_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

	FOnGameStateChanged OnGameStateChanged;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause())override;
	virtual bool ClearPause();

private:
	void SetNewGameState(EGameState NewState);

	EGameState State;
};
