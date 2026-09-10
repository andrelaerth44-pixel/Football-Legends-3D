#include "Components/FootballBallInteractionComponent.h"
#include "Ball/FootballBall.h"

UFootballBallInteractionComponent::UFootballBallInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UFootballBallInteractionComponent::IsBallInControlRange(float DistanceToBall) const
{
    return DistanceToBall >= 0.0f && DistanceToBall <= ControlRadius;
}

bool UFootballBallInteractionComponent::CanControlBall(const AFootballBall* Ball) const
{
    if (!IsValid(Ball) || !GetOwner())
    {
        return false;
    }

    return IsBallInControlRange(FVector::Dist(GetOwner()->GetActorLocation(), Ball->GetActorLocation()));
}

void UFootballBallInteractionComponent::SetControlledBall(AFootballBall* NewBall)
{
    if (NewBall && !CanControlBall(NewBall))
    {
        return;
    }

    ControlledBall = NewBall;
}

void UFootballBallInteractionComponent::ReleaseControlledBall()
{
    ControlledBall = nullptr;
}

AFootballBall* UFootballBallInteractionComponent::GetControlledBall() const
{
    return ControlledBall.Get();
}
