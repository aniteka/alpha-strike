// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/HealthBarWidget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "Components/HealthComponent.h"

void UHealthBarWidget::InitializeWidget(UHealthComponent* HealthComp)
{
	UE_LOG(LogTemp, Warning, TEXT("Start initialize Widget"));
	if (!HealthComp)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Finish initialize Widget"));
	HealthComponent = HealthComp;
	HealthComponent->OnHealthUpdate.AddUObject(this, &UHealthBarWidget::OnHealthUpdate);
	OnHealthUpdate(HealthComponent->GetHealth());

	FText TheFloatText = FText::AsNumber(HealthComponent->GetMaxHealth());
	MaxHealthLabel->SetText(TheFloatText);
}

void UHealthBarWidget::OnHealthUpdate(float value)
{	
	UE_LOG(LogTemp, Warning, TEXT("Update Health"));
	HealthBar->SetPercent(value / HealthComponent->GetMaxHealth());
	FText TheFloatText = FText::AsNumber(HealthComponent->GetHealth());
	CurrentHealthLabel->SetText(TheFloatText);
}
