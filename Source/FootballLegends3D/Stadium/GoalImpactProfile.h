#pragma once

#include "CoreMinimal.h"
#include "GoalImpactProfile.generated.h"

USTRUCT(BlueprintType)
struct FGoalImpactProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Impact", meta=(ClampMin="0.0"))
    float NetReactionStrength = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Impact", meta=(ClampMin="0.01"))
    float NetReactionDuration = 0.30f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Impact", meta=(ClampMin="0.0", ClampMax="1.0"))
    float CameraShakeStrength = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Impact", meta=(ClampMin="0.0"))
    float ReplayDelay = 0.20f;
};
