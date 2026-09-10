#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FootballPlayerController.generated.h"

UCLASS()
class FOOTBALLLEGENDS3D_API AFootballPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AFootballPlayerController();

protected:
    virtual void SetupInputComponent() override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void StartSprint();
    void StopSprint();

    bool bSprintHeld = false;
};
