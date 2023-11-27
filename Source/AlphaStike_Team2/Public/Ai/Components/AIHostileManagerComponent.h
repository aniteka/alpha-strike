// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIHostileManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChangeHostilePlayerDelegate,
	UAIHostileManagerComponent*, HostileManager,
	TSoftObjectPtr<ACharacter>, NewHostile,
	TSoftObjectPtr<ACharacter>, OldHostile);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALPHASTIKE_TEAM2_API UAIHostileManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAIHostileManagerComponent();

	TSoftObjectPtr<ACharacter> GetHostile() const { return Hostile; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnChangeHostilePlayerDelegate OnChangeHostilePlayerDelegate;
	
protected:
	virtual void BeginPlay() override;

	/**
	 * @param NewHostile If nullptr => there is no hostile
	 */
	void SetHostile(const TSoftObjectPtr<ACharacter>& NewHostile);

	ACharacter* TryToFindClosestHostile() const;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Ai", meta = (AllowPrivateAccess = 1))
	TSoftObjectPtr<ACharacter> Hostile;

	UFUNCTION()
	void OnPerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);

};
