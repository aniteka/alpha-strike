// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlphaStrikeTypes.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateStatDelegate, AController*, Killed, AController*, Causer);

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AController* Controller;

	UPROPERTY(BlueprintReadWrite)
	ETeamType TeamType = ETeamType::None;

	UPROPERTY(BlueprintReadWrite)
	int32 KillsCount = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 DeathCount = 0;
};

UCLASS()
class ALPHASTIKE_TEAM2_API AMainGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMainGameState();

protected:
	virtual void BeginPlay() override;

public:
	void AddPlayer(AController* Controller);
	UFUNCTION(BlueprintPure, Category = "Stat")
	TArray<FPlayerStat> GetTeamStat(ETeamType Team) const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnUpdateStatDelegate OnUpdateStatDelegate;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	TMap<AController*, FPlayerStat> TeamStatus;

private:
	void BindDeathDelegate(AController* Controller);
	void OnPlayerDeathCallback(AController* Damaged, AController* Causer);
	UFUNCTION()
	void OnPossessedPawnChangedCallback(APawn* OldPawn, APawn* NewPawn);
};
