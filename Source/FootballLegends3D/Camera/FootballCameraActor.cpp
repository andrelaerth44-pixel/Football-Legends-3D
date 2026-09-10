#include "Camera/FootballCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

AFootballCameraActor::AFootballCameraActor()
{
    PrimaryActorTick.bCanEverTick = true;

    if (UCameraComponent* Camera = GetCameraComponent())
    {
        Camera->FieldOfView = FieldOfView;
        Camera->bConstrainAspectRatio = false;
    }
}

void AFootballCameraActor::SetFollowTarget(AActor* NewTarget)
{
    FollowTarget = NewTarget;
}

void AFootballCameraActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!IsValid(FollowTarget))
    {
        return;
    }

    const FVector TargetLocation = FollowTarget->GetActorLocation();
    const FVector TargetForward = FollowTarget->GetActorForwardVector().GetSafeNormal2D();
    const FVector CameraLocation = TargetLocation - (TargetForward * FollowDistance) + FVector::UpVector * FollowHeight;
    const FVector LookAtLocation = TargetLocation + (TargetForward * LookAheadDistance);

    const float PositionAlpha = 1.0f - FMath::Exp(-PositionInterpSpeed * DeltaSeconds);
    const float RotationAlpha = 1.0f - FMath::Exp(-RotationInterpSpeed * DeltaSeconds);

    SetActorLocation(FMath::Lerp(GetActorLocation(), CameraLocation, PositionAlpha));

    const FRotator DesiredRotation = (LookAtLocation - GetActorLocation()).Rotation();
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaSeconds, RotationInterpSpeed));
}
