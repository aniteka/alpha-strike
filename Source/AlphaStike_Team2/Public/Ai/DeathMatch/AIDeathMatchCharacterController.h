// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "GameModes/GameModeDM.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIDeathMatchCharacterController.generated.h"

class UAIHostileManagerComponent;
class UAIRouteManagerComponent;

UCLASS()
class ALPHASTIKE_TEAM2_API AAIDeathMatchCharacterController : public AAIController
{
	GENERATED_BODY()

public:
	AAIDeathMatchCharacterController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PawnPendingDestroy(APawn* inPawn) override;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

	void SetSpawnInfo(const FBotSpawnInfo& NewSpawnInfo) { SpawnInfo = NewSpawnInfo; };
	FBotSpawnInfo GetSpawnInfo() const { return SpawnInfo; }

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	void RespawnBot();
	
protected:
	void OnDeathCallback(AController* Damaged, AController* Causer);
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UAIPerceptionComponent* BasePerceptionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UAIRouteManagerComponent* RouteManagerComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UAIHostileManagerComponent* HostileManagerComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Ai")
	UBehaviorTree* MainBehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Smooth")
	float SmoothFocusInterpSpeed = 30.f;

private:
	FRotator SmoothTargetRotation;
	FBotSpawnInfo SpawnInfo;
	bool bWillRespawn = false;
};
