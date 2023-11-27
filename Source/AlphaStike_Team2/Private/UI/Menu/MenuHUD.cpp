// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MenuHUD.h"
#include "UI/Menu/MenuWidget.h"

void AMenuHUD::BeginPlay() {
	Super::BeginPlay();

	if (MenuWidget) {
		auto Menu = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);
		if (Menu) {
			Menu->AddToViewport(0);
		}
	}
}