// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "MainGameInstance.h"
#include "UI/Menu/LevelWidget.h"
#include "UI/Menu/GameModeWidget.h"
#include "UI/Menu/TeamWidget.h"

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

	CreateTeams();
	CreateLevels();
	CreateGameModes();
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
		CurrentLevel->SetIsEnabled(false);
		CurrentLevel->SetRenderOpacity(0.5f);
		LevelsHorizontalBox->AddChild(CurrentLevel);
		Levels.Add(CurrentLevel);
	}
}

void UMenuWidget::CreateGameModes()
{
	auto GameInstance = GetGameInstance();

	if (!GameInstance) {
		return;
	}

	if (!GameModesHorizontalBox) {
		return;
	}

	GameModesHorizontalBox->ClearChildren();

	for (auto GameMode : GameInstance->GetGameModesData()) {
		auto CurrentGameMode = CreateWidget<UGameModeWidget>(GetWorld(), GameModeWidgetClass);

		if (!CurrentGameMode) {
			continue;
		}

		CurrentGameMode->SetupGameModeData(GameMode);
		CurrentGameMode->OnGameModeSelected.AddUObject(this, &UMenuWidget::SortLevelsByGameMode);
		GameModesHorizontalBox->AddChild(CurrentGameMode);
	}
}

void UMenuWidget::CreateTeams()
{
	UE_LOG(LogTemp, Error, TEXT("CreateTeam::Start"));
	auto GameInstance = GetGameInstance();

	if (!GameInstance) {
		return;
	}

	if (!TeamsHorizontalBox) {
		return;
	}

	TeamsHorizontalBox->ClearChildren();


	for (auto TeamData : GameInstance->GetTeamData()) {
		auto CurrentTeamData = CreateWidget<UTeamWidget>(GetWorld(), TeamWidgetClass);

		if (!CurrentTeamData) {
			continue;
		}

		CurrentTeamData->SetupTeamData(TeamData);
		CurrentTeamData->OnTeamSelected.AddUObject(this,&UMenuWidget::OnSelectTeam);
		CurrentTeamData->SetIsEnabled(false);
		CurrentTeamData->SetRenderOpacity(0.5f);

		Teams.Add(CurrentTeamData);
		TeamsHorizontalBox->AddChild(CurrentTeamData);
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

void UMenuWidget::SortLevelsByGameMode(FName GameModeName)
{
	if (Levels.IsEmpty()) {
		return;
	}

	for (auto Level : Levels) {
		if (GameModeName != Level->GetLevelData().LevelGameModeName) {
			Level->SetRenderOpacity(0.5f);
			Level->SetIsEnabled(false);
		}
		else {
			Level->SetRenderOpacity(1.f);
			Level->SetIsEnabled(true);
		}

		if (GameModeName == "DeathMatch") {
			for (auto Team : Teams) {
				Team->SetRenderOpacity(1.f);
				Team->SetIsEnabled(true);
			}
		}
		else {
			for (auto Team : Teams) {
				Team->SetRenderOpacity(0.5f);
				Team->SetIsEnabled(false);
			}
		}
	}
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

void UMenuWidget::OnSelectTeam(UTeamWidget* SelectedTeam)
{
	if (SelectedTeam) {
		SelectedTeam->SetRenderOpacity(1.f);
	}

	if (Teams.Num() > 1) {
		for (auto CurrentTeam : Teams) {
			if (CurrentTeam != SelectedTeam) {
				CurrentTeam->SetRenderOpacity(0.5f);
			}
		}
	}
}

UMainGameInstance* UMenuWidget::GetGameInstance() const
{
	return Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

}
