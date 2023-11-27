// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
	}	
	
	if (QuitGameButton) {
		QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGame);
	}
}

void UMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == PreloaderAnimation) {
		UGameplayStatics::OpenLevel(GetWorld(), "Level0");
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
