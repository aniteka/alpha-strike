// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetHp.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class ALPHASTIKE_TEAM2_API UBTService_GetHp : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetHp();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		FBlackboardKeySelector OutHp;

	void OnHealthUpdateCallback(float NewHp, UHealthComponent* HpComp, UBlackboardComponent* BBComp);
};
