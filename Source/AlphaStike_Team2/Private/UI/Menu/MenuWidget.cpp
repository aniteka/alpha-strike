// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "MainGameInstance.h"
#include "UI/Menu/LevelWidget.h"

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
	}	
	
	if (QuitGameButton) {
		QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGame);
	}

	if (SelectLevelButton) {
		SelectLevelButton->OnClicked.AddDynamic(this, &UMenuWidget::OnSelectLevel);
	}

	if (StartAnimation) {
		PlayAnimation(StartAnimation);
	}

	InitializeFirstLevel();
	CreateLevels();
}

void UMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == PreloaderAnimation) {
		UGameplayStatics::OpenLevel(GetWorld(), StartLevelData.LoadLevelName);
	}

}

void UMenuWidget::CreateLevels()
{
	auto GameInstance = GetGameInstance();

	if (!GameInstance) {
		return;
	}

	if (!LevelsHorizontalBox) {
		return;
	}

	LevelsHorizontalBox->ClearChildren();

	for (auto Level : GameInstance->GetLevelsData()) {
		auto CurrentLevel = CreateWidget<ULevelWidget>(GetWorld(), LevelWidgetClass);

		if (!CurrentLevel) {
			continue;
		}

		CurrentLevel->SetupLevel(Level);
		CurrentLevel->OnLevelSelected.AddUObject(this, &UMenuWidget::SetupLevel);
		LevelsHorizontalBox->AddChild(CurrentLevel);
	}
}

void UMenuWidget::InitializeFirstLevel()
{
	const auto GameInstance = GetGameInstance();

	if (!GameInstance) {
		return;
	}

	StartLevelData = GameInstance->GetLevelsData()[0];
}

void UMenuWidget::SetupLevel(const FLevelData& Data)
{
	StartLevelData = Data;
	OnStartGame();
}

void UMenuWidget::OnStartGame()
{
	PlayAnimation(PreloaderAnimation);
}

void UMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMenuWidget::OnSelectLevel()
{
	PlayAnimation(LevelsMenuAnimation);
}

UMainGameInstance* UMenuWidget::GetGameInstance() const
{
	return Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

}
