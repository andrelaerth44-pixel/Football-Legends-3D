#include "Components/FootballBallInteractionComponent.h"

UFootballBallInteractionComponent::UFootballBallInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UFootballBallInteractionComponent::IsBallInControlRange(float DistanceToBall) const
{
    return DistanceToBall >= 0.0f && DistanceToBall <= ControlRadius;
}
