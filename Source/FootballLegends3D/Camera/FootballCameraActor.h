#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FootballCameraActor.generated.h"

class UCameraComponent;

/** Broadcast-style gameplay camera that smoothly follows the controlled player. */
UCLASS()
class FOOTBALLLEGENDS3D_API AFootballCameraActor : public ACameraActor
{
    GENERATED_BODY()

public:
    AFootballCameraActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float FollowDistance = 950.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float FollowHeight = 620.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float LookAheadDistance = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float PositionInterpSpeed = 7.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float RotationInterpSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Football Camera")
    float FieldOfView = 45.0f;

    UFUNCTION(BlueprintCallable, Category="Football Camera")
    void SetFollowTarget(AActor* NewTarget);

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY()
    TObjectPtr<AActor> FollowTarget;
};
