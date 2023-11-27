// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/DeathMatch/AIDeathMatchCharacterController.h"

#include "Ai/Components/AIRouteManagerComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


AAIDeathMatchCharacterController::AAIDeathMatchCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;

	BasePerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("BasePerceptionComponent");
	RouteManagerComponent = CreateDefaultSubobject<UAIRouteManagerComponent>("RouteManagerComponent");
	
	SetPerceptionComponent(*BasePerceptionComponent);
}

void AAIDeathMatchCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(MainBehaviorTree);

	GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(
		this, &AAIDeathMatchCharacterController::OnTargetPerceptionUpdatedCallback);
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

void AAIDeathMatchCharacterController::OnTargetPerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
	{
		
	}
}

