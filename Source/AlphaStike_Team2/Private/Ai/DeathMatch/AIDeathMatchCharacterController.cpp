// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/DeathMatch/AIDeathMatchCharacterController.h"

#include "Ai/Components/AIHostileManagerComponent.h"
#include "Ai/Components/AIRouteManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


AAIDeathMatchCharacterController::AAIDeathMatchCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;

	BasePerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("BasePerceptionComponent");
	RouteManagerComponent = CreateDefaultSubobject<UAIRouteManagerComponent>("RouteManagerComponent");
	HostileManagerComponent = CreateDefaultSubobject<UAIHostileManagerComponent>("HostileManagerComponent");
	
	SetPerceptionComponent(*BasePerceptionComponent);
}

void AAIDeathMatchCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(MainBehaviorTree);
}

ETeamAttitude::Type AAIDeathMatchCharacterController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}
	return Super::GetTeamAttitudeTowards(Other);
}

void AAIDeathMatchCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIDeathMatchCharacterController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, false);
	 
	if (bUpdatePawn)
	{
		APawn* const MyPawn = GetPawn();
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();
	 
		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);
		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}
}
