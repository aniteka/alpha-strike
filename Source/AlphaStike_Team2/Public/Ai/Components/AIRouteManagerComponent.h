// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIRouteManagerComponent.generated.h"


class AAIRoute;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALPHASTIKE_TEAM2_API UAIRouteManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAIRouteManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	TSoftObjectPtr<AAIRoute> GetCurrentRoute() const { return CurrentRoute; }
	void SetCurrentRoute(const TSoftObjectPtr<AAIRoute>& NewCurrentRoute) { CurrentRoute = NewCurrentRoute; }

	UFUNCTION(BlueprintPure, Category = "Route")
	int32 GetCurrentPointIndex() const { return CurrentPointIndex; }
	UFUNCTION(BlueprintCallable, Category = "Route")
	void SetNextPointIndex();

	UFUNCTION(BlueprintPure, Category = "Route")
	bool IsLastPointIndex() const;
	
	UFUNCTION(BlueprintPure, Category = "Route")
	FVector GetCurrentLocation() const;

	void TryGetRouteFromGM();
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Route")
	TSoftObjectPtr<AAIRoute> CurrentRoute = nullptr;

	int32 CurrentPointIndex = 0;
	
};
