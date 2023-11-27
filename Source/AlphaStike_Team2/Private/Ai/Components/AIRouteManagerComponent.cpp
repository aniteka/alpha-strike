// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Components/AIRouteManagerComponent.h"

#include "AITypes.h"
#include "Ai/AIRoute.h"
#include "Components/SplineComponent.h"


UAIRouteManagerComponent::UAIRouteManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UAIRouteManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAIRouteManagerComponent::SetNextPointIndex()
{
	if(IsLastPointIndex())
		return;
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

