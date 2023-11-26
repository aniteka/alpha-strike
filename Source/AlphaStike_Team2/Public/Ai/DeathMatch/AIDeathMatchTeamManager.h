// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlphaStrikeTypes.h"
#include "AIDeathMatchTeamManager.generated.h"

class AAIController;
class UBillboardComponent;

UCLASS()
class ALPHASTIKE_TEAM2_API AAIDeathMatchTeamManager : public AActor
{
	GENERATED_BODY()

public:
	AAIDeathMatchTeamManager();

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

	TArray<TSoftObjectPtr<AController>> Team;
	
	ETeamType TeamType = ETeamType::None;
};
