// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Services/BTService_Shooting.h"

#include "AIController.h"
#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Weapons/Rifle.h"
#include "Weapons/RobotHandWeapon.h"

struct FNodeMemory_Shooting
{
	UWeaponComponent* WeaponComponent;
	FTimerHandle ShootingTimerHandle;
};

UBTService_Shooting::UBTService_Shooting()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	NodeName = "Shooting";
}

void UBTService_Shooting::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_Shooting>(NodeMemory);
	check(NodeInstance);
	
	const auto Controller = Cast<AController>(OwnerComp.GetOwner());
	check(Controller);
	const auto Pawn = Controller->GetPawn();
	check(Pawn);
	NodeInstance->WeaponComponent = Pawn->GetComponentByClass<UWeaponComponent>();
	check(NodeInstance->WeaponComponent);

	NodeInstance->WeaponComponent->OnEndWeaponReloading.AddDynamic(this, &UBTService_Shooting::StartFire);

	StartFire(NodeInstance->WeaponComponent);
}

void UBTService_Shooting::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_Shooting>(NodeMemory);
	check(NodeInstance);

	if(NodeInstance->WeaponComponent->GetCurrentWeapon()->IsA(ARobotHandWeapon::StaticClass()))
		NodeInstance->WeaponComponent->StopFire();

	if(NodeInstance->WeaponComponent->GetCurrentWeapon()->IsA(ARifle::StaticClass()))
		GetWorld()->GetTimerManager().ClearTimer(NodeInstance->ShootingTimerHandle);

	NodeInstance->WeaponComponent->OnEndWeaponReloading.RemoveDynamic(this, &UBTService_Shooting::StartFire);
}

uint16 UBTService_Shooting::GetInstanceMemorySize() const
{
	return sizeof(FNodeMemory_Shooting);
}

void UBTService_Shooting::StartFire(UWeaponComponent* WeaponComponent)
{
	if(!WeaponComponent)
		return;

	ChooseWeapon(WeaponComponent);
	
	if(WeaponComponent->GetCurrentWeapon()->IsA(ARobotHandWeapon::StaticClass()))
		WeaponComponent->StartFire();

	if(WeaponComponent->GetCurrentWeapon()->IsA(ARifle::StaticClass()))
	{
		const auto Pawn = WeaponComponent->GetOwner<APawn>();
		if(!Pawn)
			return;
		const auto AiController = Pawn->GetController<AAIController>();
		if(!AiController)
			return;
		
		UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(AiController->GetBrainComponent());
		if(!OwnerComp)
			return;

		const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_Shooting>(
			OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this)));
		if(!NodeInstance)
			return;

		Shot(NodeInstance);
	}
}

void UBTService_Shooting::Shot(FNodeMemory_Shooting* const NodeInstance)
{
	if(!NodeInstance || !NodeInstance->WeaponComponent)
		return;

	if(NodeInstance->WeaponComponent->CanFire())
	{
		NodeInstance->WeaponComponent->StartFire();
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UBTService_Shooting::Shot, NodeInstance);
		GetWorld()->GetTimerManager().SetTimer(NodeInstance->ShootingTimerHandle, TimerDelegate, GetShotInterval(), false);
	}
	
	const auto CurrentWeapon = NodeInstance->WeaponComponent->GetCurrentWeapon();
	if(!CurrentWeapon)
		return;
	const auto AmmoData = CurrentWeapon->GetAmmoData();

	if(AmmoData.BulletsNum == 0 && AmmoData.ClipNum != 0)
	{
		NodeInstance->WeaponComponent->Reload();
	}
	
	if(AmmoData.BulletsNum == 0 && AmmoData.ClipNum == 0 && !AmmoData.Infinite)
	{
		GetWorld()->GetTimerManager().ClearTimer(NodeInstance->ShootingTimerHandle);
		StartFire(NodeInstance->WeaponComponent); // Called ChooseWeapon
	}
}

void UBTService_Shooting::ChooseWeapon(UWeaponComponent* WeaponComponent)
{
	const auto& Weapons =  WeaponComponent->GetWeapons();
	for(int32 i = 0; i < Weapons.Num(); ++i)
	{
		const auto AmmoData = Weapons[i]->GetAmmoData();
		if(AmmoData.Infinite || (AmmoData.BulletsNum != 0 || AmmoData.ClipNum != 0))
		{
			if(Weapons[i] != WeaponComponent->GetCurrentWeapon())
			{
				WeaponComponent->SetWeaponOnIndex(i);
				StartFire(WeaponComponent);
			}
			break;
		}
	}
}

float UBTService_Shooting::GetShotInterval() const
{
	return FMath::RandRange(ShootInterval - ShootIntervalRandomDeviation, ShootInterval + ShootIntervalRandomDeviation);
}
