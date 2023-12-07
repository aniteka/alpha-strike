// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(5.f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1500.f;
}

void AProjectile::SetupDirection(const FVector& ShotDirection)
{
	Direction = ShotDirection;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(ProjectileMovementComponent);

	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->InitialSpeed * Direction;
	SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	SetLifeSpan(10.f);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) {
		return;
	}

	auto Player = Cast<APawn>(GetOwner());
	if (!Player) {
		return;
	}

	auto Controller = Player->GetController();

	if (!Controller) {
		return;
	}

	UGameplayStatics::ApplyRadialDamage
	(
		GetWorld(),
		Damage,
		GetActorLocation(),
		DamageRadius,
		UDamageType::StaticClass(),
		{},
		this,
		Controller,
		DoFullDamage
	);

	if(ExplosionParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());

	if(ExplosionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), GetActorRotation());
	
	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.f);

	Destroy();
}


