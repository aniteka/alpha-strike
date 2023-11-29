// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Components/AIHostileManagerComponent.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UAIHostileManagerComponent::UAIHostileManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIHostileManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto Controller = GetOwner<AAIController>();
	check(Controller && Controller->GetPerceptionComponent());

	Controller->GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(this, &UAIHostileManagerComponent::OnPerceptionUpdatedCallback);
	Controller->GetPerceptionComponent()->OnTargetPerceptionForgotten.AddDynamic(this, &UAIHostileManagerComponent::OnPerceptionForgottenCallback);
}

void UAIHostileManagerComponent::SetHostile(const TSoftObjectPtr<ACharacter>& NewHostile)
{
	const auto OldHostile = Hostile;
	Hostile = NewHostile;
	OnChangeHostilePlayerDelegate.Broadcast(this, Hostile, OldHostile);
}

ACharacter* UAIHostileManagerComponent::TryToFindClosestHostile() const
{
	const auto Controller = GetOwner<AAIController>();
	const auto PercComp = Controller->GetPerceptionComponent();
	check(Controller && PercComp && Controller->GetPawn());

	TArray<AActor*> HostileCharacters;
	PercComp->GetCurrentlyPerceivedActors(nullptr, HostileCharacters);
	
	float Dist;
	return Cast<ACharacter>(UGameplayStatics::FindNearestActor(Controller->GetPawn()->GetActorLocation(), HostileCharacters, Dist));
}

void UAIHostileManagerComponent::OnPerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(!UpdateInfo.Target.IsValid())
		return;

	if(!GetHostile() && UpdateInfo.Stimulus.WasSuccessfullySensed())
		SetHostile(UpdateInfo.Target.Get());
}

void UAIHostileManagerComponent::OnPerceptionForgottenCallback(AActor* Actor)
{
	if(!Actor || !GetHostile() || GetHostile() != Actor)
		return;

	const auto Controller = GetOwner<AAIController>();
	const auto PercComp = Controller->GetPerceptionComponent();
	check(Controller && PercComp);
	
	const auto Info = PercComp->GetActorInfo(*Actor);
	if(!Info || !Info->HasAnyKnownStimulus())
		SetHostile(TryToFindClosestHostile());
}
