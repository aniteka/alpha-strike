// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
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
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIPerceptionComponent* BasePerceptionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIRouteManagerComponent* RouteManagerComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIHostileManagerComponent* HostileManagerComponent;
		
	UPROPERTY(EditAnywhere, Category = "Ai")
		UBehaviorTree* MainBehaviorTree;

};
