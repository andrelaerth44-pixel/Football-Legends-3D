#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FootballGoalReplayCameraActor.generated.h"

class AFootballBall;

UCLASS()
class FOOTBALLLEGENDS3D_API AFootballGoalReplayCameraActor : public ACameraActor
{
    GENERATED_BODY()

public:
    AFootballGoalReplayCameraActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera")
    float Distance = 520.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera")
    float Height = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera")
    float LookAhead = 80.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera")
    float PositionInterpSpeed = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera")
    float RotationInterpSpeed = 7.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay Camera", meta=(ClampMin="20.0", ClampMax="100.0"))
    float FieldOfView = 55.0f;

    UFUNCTION(BlueprintCallable, Category="Goal Replay Camera")
    void SetReplayBall(AFootballBall* Ball);

    UFUNCTION(BlueprintCallable, Category="Goal Replay Camera")
    void SetReplayActive(bool bActive);

    UFUNCTION(BlueprintPure, Category="Goal Replay Camera")
    bool IsReplayActive() const;

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY()
    TObjectPtr<AFootballBall> ReplayBall;

    bool bReplayActive = false;
};
