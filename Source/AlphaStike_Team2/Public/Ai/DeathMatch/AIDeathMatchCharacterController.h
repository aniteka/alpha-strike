// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "AIDeathMatchCharacterController.generated.h"

UCLASS()
class ALPHASTIKE_TEAM2_API AAIDeathMatchCharacterController : public AAIController
{
	GENERATED_BODY()

public:
	AAIDeathMatchCharacterController();

protected:
	virtual void BeginPlay() override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
public:
	virtual void Tick(float DeltaTime) override;
};
