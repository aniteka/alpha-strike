// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BasePickupActor.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABasePickupActor::ABasePickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColliderComponent = CreateDefaultSubobject<USphereComponent>("Collider Sphere");
	SetRootComponent(ColliderComponent);
	ColliderComponent->SetGenerateOverlapEvents(true);
	ColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ColliderComponent->OnComponentBeginOverlap.AddDynamic(
		this, &ABasePickupActor::OnBeginOverlapComponentEvent
	);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComponent->SetupAttachment(ColliderComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetGenerateOverlapEvents(false);

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Component");
}

void ABasePickupActor::OnBeginOverlapComponentEvent(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
)
{		
	//UE_LOG(LogTemp, Warning, TEXT("------------OVERLAP------------"));

	Character = Cast<ABaseCharacter>(OtherActor);
	if (!Character)
	{	
		//UE_LOG(LogTemp, Warning, TEXT("------------NOT_CHARACTER------------"));
		return;
	}

	Pickup();
	Destroy();
}

void ABasePickupActor::Pickup()
{
	//UE_LOG(LogTemp, Warning,TEXT("------------PICKUP------------"));
}




