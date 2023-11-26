#pragma once
#include "GenericTeamAgentInterface.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Red,
	Blue,
	None = UINT8_MAX UMETA(Hidden)
};

