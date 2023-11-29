// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/DeathMatch/DEPRECATED_AIDeathMatchTeamManager.h"

#include "AIController.h"
#include "Ai/Components/AIRouteManagerComponent.h"
#include "Components/BillboardComponent.h"


ADEPRECATED_AIDeathMatchTeamManager::ADEPRECATED_AIDeathMatchTeamManager()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<UBillboardComponent>("Base Component");
	SetRootComponent(BaseComponent);
#if WITH_EDITOR
	if(const auto Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_Solver.S_Solver")))
		BaseComponent->SetSprite(Texture);
#endif
}

void ADEPRECATED_AIDeathMatchTeamManager::BeginPlay()
{
	Super::BeginPlay();

	/*
	 * TODO Refactor next
	 */
	if(PossibleRoutes.Num() == 0)
		return;
	for (const auto& Controller : Team)
	{
		if(!Controller.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller from team is not valid"));
			continue;
		}
		if(const auto RouteManagerComponent = Controller->GetComponentByClass<UAIRouteManagerComponent>())
			RouteManagerComponent->SetCurrentRoute(PossibleRoutes[FMath::RandRange(0, PossibleRoutes.Num() - 1)]);
	}
}

void ADEPRECATED_AIDeathMatchTeamManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADEPRECATED_AIDeathMatchTeamManager::AddTeamMember(TSoftObjectPtr<AController> Controller)
{
	check(Controller.IsValid());
	Team.Add(Controller);
	if(const auto TeamAgent = Cast<IGenericTeamAgentInterface>(Controller.Get()))
		TeamAgent->SetGenericTeamId({static_cast<uint8>(TeamType)});
}

