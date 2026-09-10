#pragma once

#include "CoreMinimal.h"
#include "FootballTeamTypes.generated.h"

UENUM(BlueprintType)
enum class EFootballPlayerRole : uint8
{
    Goalkeeper,
    Defender,
    Midfielder,
    Forward
};

USTRUCT(BlueprintType)
struct FFootballFormationSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFootballPlayerRole Role = EFootballPlayerRole::Midfielder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ShirtNumber = 0;

    /** Normalized pitch coordinates: X forward/back, Y left/right. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D NormalizedPosition = FVector2D::ZeroVector;
};

USTRUCT(BlueprintType)
struct FFootballTeamDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TeamId = TEXT("team_default");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FFootballFormationSlot> Formation;
};
