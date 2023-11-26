// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/DeathMatch/AIDeathMatchCharacterController.h"


AAIDeathMatchCharacterController::AAIDeathMatchCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIDeathMatchCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
}

ETeamAttitude::Type AAIDeathMatchCharacterController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}
	return Super::GetTeamAttitudeTowards(Other);
}

void AAIDeathMatchCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

