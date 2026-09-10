#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FootballPlayerController.generated.h"

class AFootballCameraActor;
class AFootballBall;

enum class EFootballKickType : uint8;

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

    void StartPass();
    void ReleasePass();
    void StartShot();
    void ReleaseShot();
    void Kick(EFootballKickType KickType, float Charge);
    AFootballBall* FindControllableBall() const;
    FVector GetKickDirection() const;
    void UpdateMovementSpeed();

    UPROPERTY()
    TObjectPtr<AFootballCameraActor> FootballCamera;

    bool bSprintHeld = false;
    bool bPassHeld = false;
    bool bShotHeld = false;
    float PassCharge = 0.0f;
    float ShotCharge = 0.0f;

    UPROPERTY(EditAnywhere, Category="Football|Input", meta=(ClampMin="0.1"))
    float ChargeRate = 1.6f;
};
