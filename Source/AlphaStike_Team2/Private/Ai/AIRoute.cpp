// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/AIRoute.h"

#include "Components/SplineComponent.h"


AAIRoute::AAIRoute()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SetRootComponent(SplineComponent);
}

