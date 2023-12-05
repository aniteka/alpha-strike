// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Components/AIRouteManagerComponent.h"

#include "AITypes.h"
#include "Ai/AIRoute.h"
#include "Components/SplineComponent.h"
#include "GameModes/MainGameMode.h"


UAIRouteManagerComponent::UAIRouteManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAIRouteManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	TryGetRouteFromGM();
}

void UAIRouteManagerComponent::SetNextPointIndex()
{
	if(IsLastPointIndex() || !CurrentRoute.IsValid())
	{
		TryGetRouteFromGM();
		return;
	}
	CurrentPointIndex += 1;
}

bool UAIRouteManagerComponent::IsLastPointIndex() const
{
	return GetCurrentPointIndex() >= CurrentRoute->GetSplineComponent()->GetNumberOfSplinePoints() - 1;
}

FVector UAIRouteManagerComponent::GetCurrentLocation() const
{
	if(!CurrentRoute.IsValid() || !CurrentRoute->GetSplineComponent())
		return FAISystem::InvalidLocation;
	return CurrentRoute->GetSplineComponent()->GetLocationAtSplinePoint(CurrentPointIndex, ESplineCoordinateSpace::World);
}

void UAIRouteManagerComponent::TryGetRouteFromGM()
{
	const auto GameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
	const auto TeamAgent = GetOwner<IGenericTeamAgentInterface>(); 
	check(GameMode);
	CurrentRoute = GameMode->GetRouteForTeam(static_cast<ETeamType>(TeamAgent->GetGenericTeamId().GetId()));
	CurrentPointIndex = 0;
}

