// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlphaStrikeTypes.h"
#include "DEPRECATED_AIDeathMatchTeamManager.generated.h"

class AAIRoute;
class AAIController;
class UBillboardComponent;

UCLASS(Deprecated)
class ALPHASTIKE_TEAM2_API ADEPRECATED_AIDeathMatchTeamManager : public AActor
{
	GENERATED_BODY()

public:
	ADEPRECATED_AIDeathMatchTeamManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetTeamType(ETeamType NewTeamType) { TeamType = NewTeamType; }
	ETeamType GetTeamType() const { return TeamType; }

	void AddTeamMember(TSoftObjectPtr<AController> Controller);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UBillboardComponent* BaseComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai|Routes")
		TArray<TSoftObjectPtr<AAIRoute>> PossibleRoutes;
	
	TArray<TSoftObjectPtr<AController>> Team;
	
	ETeamType TeamType = ETeamType::None;
};
