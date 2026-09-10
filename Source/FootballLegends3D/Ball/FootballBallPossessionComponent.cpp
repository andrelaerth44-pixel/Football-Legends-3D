#include "Ball/FootballBallPossessionComponent.h"
#include "Ball/FootballBall.h"
#include "Components/PrimitiveComponent.h"

UFootballBallPossessionComponent::UFootballBallPossessionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool UFootballBallPossessionComponent::AcquireBall(AFootballBall* Ball)
{
    if (!IsValid(Ball) || !GetOwner())
    {
        return false;
    }

    if (FVector::Dist(GetOwner()->GetActorLocation(), Ball->GetActorLocation()) > MaxControlDistance)
    {
        return false;
    }

    ControlledBall = Ball;
    Ball->StopBall();
    return true;
}

void UFootballBallPossessionComponent::ReleaseBall()
{
    ControlledBall = nullptr;
}

bool UFootballBallPossessionComponent::HasBall() const
{
    return IsValid(ControlledBall);
}

void UFootballBallPossessionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!IsValid(ControlledBall) || !GetOwner())
    {
        ControlledBall = nullptr;
        return;
    }

    const FVector OwnerLocation = GetOwner()->GetActorLocation();
    const FVector Forward = GetOwner()->GetActorForwardVector().GetSafeNormal2D();
    const FVector DesiredLocation = OwnerLocation + Forward * TouchDistance + FVector(0.0f, 0.0f, 20.0f);
    const FVector NewLocation = FMath::VInterpTo(ControlledBall->GetActorLocation(), DesiredLocation, DeltaTime, ControlSpeed);

    ControlledBall->SetActorLocation(NewLocation, false);
}
