#pragma once

#include "CoreMinimal.h"
#include "FootballMatchTypes.generated.h"

UENUM(BlueprintType)
enum class EFootballTeamSide : uint8
{
    Home UMETA(DisplayName="Home"),
    Away UMETA(DisplayName="Away")
};
