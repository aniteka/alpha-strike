// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Shooting.generated.h"

UCLASS()
class ALPHASTIKE_TEAM2_API UBTService_Shooting : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Shooting();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Options")
	float ShootInterval = 0.5f;
	UPROPERTY(EditAnywhere, DisplayName = "RandomDeviation", Category = "Options")
	float ShootIntervalRandomDeviation = 0.1f;
};
