#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballBallPossessionComponent.generated.h"

class AFootballBall;
class UFootballBallInteractionComponent;

/** Lightweight possession controller. Keeps the ball near the player's preferred touch point while dribbling. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballBallPossessionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballBallPossessionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
    float TouchDistance = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
    float MaxControlDistance = 160.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
    float ControlSpeed = 11.0f;

    UFUNCTION(BlueprintCallable, Category="Possession")
    bool AcquireBall(AFootballBall* Ball);

    UFUNCTION(BlueprintCallable, Category="Possession")
    void ReleaseBall();

    UFUNCTION(BlueprintPure, Category="Possession")
    bool HasBall() const;

    UFUNCTION(BlueprintPure, Category="Possession")
    AFootballBall* GetControlledBall() const;

protected:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    TObjectPtr<AFootballBall> ControlledBall;
};
