// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Services/BTService_GameplayFocus.h"

#include "AIController.h"

UBTService_GameplayFocus::UBTService_GameplayFocus()
{
	NodeName = "Set gameplay focus";
	FocusPriority = EAIFocusPriority::Gameplay;
}
