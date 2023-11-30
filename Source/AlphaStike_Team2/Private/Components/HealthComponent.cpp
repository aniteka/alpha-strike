// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UHealthComponent::DamageTaken(AActor* DamageActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	Health -= Damage;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Health);

	OnHealthUpdate.Broadcast(Health);

	if (Health <= 0.f)
	{
		HandleDestroy(DamageActor);
	}

	// For AIPerception
	if(const auto DamagedPawn = Cast<APawn>(DamageActor); DamagedPawn && DamagedPawn->GetController() && Instigator->GetPawn())
		UAISense_Damage::ReportDamageEvent(GetWorld(), DamagedPawn->GetController(), Instigator->GetPawn(), Damage,
		                                   Instigator->GetPawn()->GetActorLocation(),
		                                   DamageActor->GetActorLocation());
}

void UHealthComponent::HandleDestroy(AActor* DamageActor)
{
	
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

