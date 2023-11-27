// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIRoute.generated.h"

class USplineComponent;

UCLASS()
class ALPHASTIKE_TEAM2_API AAIRoute : public AActor
{
	GENERATED_BODY()

public:
	AAIRoute();

	USplineComponent* GetSplineComponent() const { return SplineComponent; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	USplineComponent* SplineComponent;

};
