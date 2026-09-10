#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FootballPlayerController.generated.h"

class AFootballCameraActor;

UCLASS()
class FOOTBALLLEGENDS3D_API AFootballPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AFootballPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void StartSprint();
    void StopSprint();
    void UpdateMovementSpeed();

    UPROPERTY()
    TObjectPtr<AFootballCameraActor> FootballCamera;

    bool bSprintHeld = false;
};
