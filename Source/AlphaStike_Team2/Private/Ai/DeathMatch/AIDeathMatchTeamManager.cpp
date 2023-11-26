// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/DeathMatch/AIDeathMatchTeamManager.h"

#include "AIController.h"
#include "Components/BillboardComponent.h"


AAIDeathMatchTeamManager::AAIDeathMatchTeamManager()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<UBillboardComponent>("Base Component");
	SetRootComponent(BaseComponent);
#if WITH_EDITOR
	if(const auto Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_Solver.S_Solver")))
		BaseComponent->SetSprite(Texture);
#endif
}

void AAIDeathMatchTeamManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIDeathMatchTeamManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIDeathMatchTeamManager::AddTeamMember(TSoftObjectPtr<AController> Controller)
{
	check(Controller.IsValid());
	Team.Add(Controller);
	if(const auto TeamAgent = Cast<IGenericTeamAgentInterface>(Controller.Get()))
		TeamAgent->SetGenericTeamId({static_cast<uint8>(TeamType)});
}

