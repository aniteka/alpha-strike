// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/TeamWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MainGameInstance.h"

void UTeamWidget::SetupTeamData(ETeamType NewTeamType)
{
	TeamType = NewTeamType;

	switch (TeamType)
	{
	case ETeamType::Red:
		TeamImage->SetColorAndOpacity(FLinearColor::Red);
		break;
	case ETeamType::Blue:
		TeamImage->SetColorAndOpacity(FLinearColor::Blue);
		break;
	default:
		break;
	}
}

void UTeamWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TeamButton) {
		TeamButton->OnClicked.AddDynamic(this, &UTeamWidget::OnTeamButtonClicked);
	}
}

void UTeamWidget::OnTeamButtonClicked()
{
	const auto GameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return;
	}

	GameInstance->SetPlayerTeamType(TeamType);
	OnTeamSelected.Broadcast(this);

}
