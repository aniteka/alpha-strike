// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Services/BTService_GetHp.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"

struct FNodeMemory_GetHp
{
	FDelegateHandle OnHealthUpdateDelegateHandle;
};

UBTService_GetHp::UBTService_GetHp()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	NodeName = "Set Heal Points Status";
}

void UBTService_GetHp::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_GetHp>(NodeMemory);
	check(NodeInstance);
	
	const auto Controller = Cast<AController>(OwnerComp.GetOwner());
	check(Controller);
	const auto Pawn = Controller->GetPawn();
	check(Pawn);
	const auto HpComp = Pawn->GetComponentByClass<UHealthComponent>();
	check(HpComp);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OutHp.SelectedKeyName, HpComp->GetHealtPercent());
	
	NodeInstance->OnHealthUpdateDelegateHandle = HpComp->OnHealthUpdate.AddUObject(
		this, &UBTService_GetHp::OnHealthUpdateCallback,
		HpComp,
		OwnerComp.GetBlackboardComponent());
}

void UBTService_GetHp::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto NodeInstance = CastInstanceNodeMemory<FNodeMemory_GetHp>(NodeMemory);
	check(NodeInstance);
	
	const auto Controller = Cast<AController>(OwnerComp.GetOwner());
	check(Controller);
	const auto Pawn = Controller->GetPawn();
	if(!Pawn)
		return;
	const auto HpComp = Pawn->GetComponentByClass<UHealthComponent>();
	if(!HpComp)
		return;

	HpComp->OnHealthUpdate.Remove(NodeInstance->OnHealthUpdateDelegateHandle);
}

uint16 UBTService_GetHp::GetInstanceMemorySize() const
{
	return sizeof(FNodeMemory_GetHp);
}
void UBTService_GetHp::OnHealthUpdateCallback(float NewHp, UHealthComponent* HpComp, UBlackboardComponent* BBComp)
{
	BBComp->SetValueAsFloat(OutHp.SelectedKeyName, HpComp->GetHealtPercent());
}



