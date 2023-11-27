// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MenuGameMode.h"
#include "UI/Menu/MenuHUD.h"

AMenuGameMode::AMenuGameMode() {
	HUDClass = AMenuHUD::StaticClass();
}