#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballPlayerMovementComponent.generated.h"

/** Tunable football-specific movement layer kept separate from character presentation. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballPlayerMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballPlayerMovementComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin="0.0"))
    float JogSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin="0.0"))
    float SprintSpeed = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin="0.0"))
    float Acceleration = 1800.0f;

    UFUNCTION(BlueprintPure, Category="Movement")
    float GetTargetSpeed(bool bSprinting) const;
};
