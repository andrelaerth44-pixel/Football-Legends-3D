#include "Characters/FootballPlayerMovementComponent.h"

UFootballPlayerMovementComponent::UFootballPlayerMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float UFootballPlayerMovementComponent::GetTargetSpeed(bool bSprinting) const
{
    return bSprinting ? SprintSpeed : JogSpeed;
}
