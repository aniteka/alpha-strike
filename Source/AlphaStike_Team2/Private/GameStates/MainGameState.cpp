// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/MainGameState.h"

#include "Character/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"


AMainGameState::AMainGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AController::StaticClass(), Actors);

	for (const auto& Actor : Actors)
	{
		const auto Controller = Cast<AController>(Actor);
		check(Controller);
		AddPlayer(Controller);
		BindDeathDelegate(Controller);
	}
}

void AMainGameState::AddPlayer(AController* Controller)
{
	if(!Controller)
		return;

	Controller->OnPossessedPawnChanged.AddDynamic(this, &AMainGameState::OnPossessedPawnChangedCallback);
	
	const auto TeamAgent = Cast<IGenericTeamAgentInterface>(Controller);
	if(!TeamAgent)
		return;

	FPlayerStat PlayerStat;
	PlayerStat.Controller = Controller;
	PlayerStat.TeamType = static_cast<ETeamType>(TeamAgent->GetGenericTeamId().GetId());

	TeamStatus.Add(Controller, PlayerStat);
}

TArray<FPlayerStat> AMainGameState::GetTeamStat(ETeamType Team) const
{
	TArray<FPlayerStat> ToRet;

	TArray<FPlayerStat> Temp;
	TeamStatus.GenerateValueArray(Temp);

	for (const auto& PlayerStat : Temp)
	{
		if(PlayerStat.TeamType == Team)
			ToRet.Add(PlayerStat);
	}
	return ToRet;
}

void AMainGameState::BindDeathDelegate(AController* Controller)
{
	const auto Pawn = Controller->GetPawn();
	check(Pawn);
	const auto HpComp = Pawn->GetComponentByClass<UHealthComponent>();
	check(HpComp);

	HpComp->OnDeathDelegate.AddUObject(this, &AMainGameState::OnPlayerDeathCallback);
}

void AMainGameState::OnPlayerDeathCallback(AController* Damaged, AController* Causer)
{
	check(TeamStatus.Contains(Damaged) && TeamStatus.Contains(Causer));
	TeamStatus[Damaged].DeathCount += 1;
	TeamStatus[Causer].KillsCount += 1;
	OnUpdateStatDelegate.Broadcast(Damaged, Causer);
}

void AMainGameState::OnPossessedPawnChangedCallback(APawn* OldPawn, APawn* NewPawn)
{
	if(!NewPawn)
		return;
	const auto Controller = NewPawn->GetController();
	if(!Controller)
		return;
	BindDeathDelegate(Controller);
}

