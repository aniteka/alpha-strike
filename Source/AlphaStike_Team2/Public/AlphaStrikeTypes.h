#pragma once
#include "GenericTeamAgentInterface.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Hostile,
	Red,
	Blue,
	None = UINT8_MAX UMETA(Hidden)
};

inline ETeamAttitude::Type MainTeamAttitudeSolver(FGenericTeamId A, FGenericTeamId B)
{
	if(static_cast<ETeamType>(A.GetId()) == ETeamType::Hostile || static_cast<ETeamType>(B.GetId()) == ETeamType::Hostile)
		return ETeamAttitude::Hostile;
	return A != B ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
}
