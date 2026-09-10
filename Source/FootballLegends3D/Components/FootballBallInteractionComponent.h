#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballBallInteractionComponent.generated.h"

class AFootballBall;

/** Entry point for receiving, dribbling, passing and shooting interactions. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballBallInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballBallInteractionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball", meta=(ClampMin="0.0"))
    float ControlRadius = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball", meta=(ClampMin="0.0"))
    float PreferredTouchDistance = 55.0f;

    UFUNCTION(BlueprintPure, Category="Ball")
    bool IsBallInControlRange(float DistanceToBall) const;

    UFUNCTION(BlueprintPure, Category="Ball")
    bool CanControlBall(const AFootballBall* Ball) const;

    UFUNCTION(BlueprintCallable, Category="Ball")
    void SetControlledBall(AFootballBall* NewBall);

    UFUNCTION(BlueprintCallable, Category="Ball")
    void ReleaseControlledBall();

    UFUNCTION(BlueprintPure, Category="Ball")
    AFootballBall* GetControlledBall() const;

private:
    UPROPERTY()
    TObjectPtr<AFootballBall> ControlledBall;
};
