// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Services/BTService_Shooting.h"

#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Weapons/RobotHandWeapon.h"

struct FNodeMemory_Shooting
{
	UWeaponComponent* WeaponComponent;
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
	if(WeaponComponent->GetCurrentWeapon()->IsA(ARobotHandWeapon::StaticClass()))
		WeaponComponent->StartFire();
}
