#include "Camera/FootballGoalReplayCameraActor.h"
#include "Camera/CameraComponent.h"
#include "Ball/FootballBall.h"
#include "GameFramework/Actor.h"

AFootballGoalReplayCameraActor::AFootballGoalReplayCameraActor()
{
    PrimaryActorTick.bCanEverTick = true;

    if (UCameraComponent* Camera = GetCameraComponent())
    {
        Camera->FieldOfView = FieldOfView;
        Camera->bConstrainAspectRatio = false;
    }
}

void AFootballGoalReplayCameraActor::SetReplayBall(AFootballBall* Ball)
{
    ReplayBall = Ball;
}

void AFootballGoalReplayCameraActor::SetReplayActive(bool bActive)
{
    bReplayActive = bActive;

    if (UCameraComponent* Camera = GetCameraComponent())
    {
        Camera->FieldOfView = FieldOfView;
    }
}

bool AFootballGoalReplayCameraActor::IsReplayActive() const
{
    return bReplayActive;
}

void AFootballGoalReplayCameraActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bReplayActive || !IsValid(ReplayBall))
    {
        return;
    }

    const FVector BallLocation = ReplayBall->GetActorLocation();
    FVector BallDirection = ReplayBall->BallMesh
        ? ReplayBall->BallMesh->GetPhysicsLinearVelocity().GetSafeNormal2D()
        : ReplayBall->GetActorForwardVector().GetSafeNormal2D();

    if (BallDirection.IsNearlyZero())
    {
        BallDirection = ReplayBall->GetActorForwardVector().GetSafeNormal2D();
    }

    const FVector SideOffset = FVector::CrossProduct(FVector::UpVector, BallDirection).GetSafeNormal() * Distance;
    const FVector DesiredLocation = BallLocation - SideOffset + FVector::UpVector * Height;
    const FVector LookAt = BallLocation + BallDirection * LookAhead;

    const float PositionAlpha = 1.0f - FMath::Exp(-PositionInterpSpeed * FMath::Max(0.0f, DeltaSeconds));
    SetActorLocation(FMath::Lerp(GetActorLocation(), DesiredLocation, PositionAlpha));

    const FRotator DesiredRotation = (LookAt - GetActorLocation()).Rotation();
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaSeconds, RotationInterpSpeed));
}
