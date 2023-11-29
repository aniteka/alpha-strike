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

	if(NodeInstance->WeaponComponent->GetCurrentWeapon()->IsA(ARobotHandWeapon::StaticClass()))
		NodeInstance->WeaponComponent->StartFire();
}

void UBTService_Shooting::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_Shooting>(NodeMemory);
	check(NodeInstance);

	if(NodeInstance->WeaponComponent->GetCurrentWeapon()->IsA(ARobotHandWeapon::StaticClass()))
		NodeInstance->WeaponComponent->StopFire();
}

uint16 UBTService_Shooting::GetInstanceMemorySize() const
{
	return sizeof(FNodeMemory_Shooting);
}
