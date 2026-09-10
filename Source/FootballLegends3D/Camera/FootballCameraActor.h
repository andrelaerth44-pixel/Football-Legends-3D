#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FootballCameraActor.generated.h"

class UCameraComponent;

/** Broadcast-style gameplay camera. Runtime tuning stays in one place. */
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
};
