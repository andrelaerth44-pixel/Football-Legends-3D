#include "Ball/FootballBallGameplayComponent.h"
#include "Ball/FootballBall.h"

UFootballBallGameplayComponent::UFootballBallGameplayComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float UFootballBallGameplayComponent::GetKickSpeed(EFootballKickType KickType, float Charge) const
{
    const float SafeCharge = FMath::Clamp(Charge, 0.0f, 1.0f);

    switch (KickType)
    {
    case EFootballKickType::ShortPass:
        return ShortPassSpeed * FMath::Lerp(0.75f, 1.0f, SafeCharge);
    case EFootballKickType::LongPass:
        return LongPassSpeed * FMath::Lerp(0.75f, 1.0f, SafeCharge);
    case EFootballKickType::ThroughPass:
        return ThroughPassSpeed * FMath::Lerp(0.75f, 1.0f, SafeCharge);
    case EFootballKickType::Cross:
        return CrossSpeed * FMath::Lerp(0.75f, 1.0f, SafeCharge);
    case EFootballKickType::NormalShot:
        return NormalShotSpeed * FMath::Lerp(0.70f, 1.0f, SafeCharge);
    case EFootballKickType::PowerShot:
        return PowerShotSpeed * FMath::Lerp(0.55f, 1.0f, SafeCharge);
    default:
        return 0.0f;
    }
}

bool UFootballBallGameplayComponent::KickBall(AFootballBall* Ball, const FVector& Direction, EFootballKickType KickType, float Charge)
{
    if (!IsValid(Ball))
    {
        return false;
    }

    const FVector SafeDirection = Direction.GetSafeNormal();
    if (SafeDirection.IsNearlyZero())
    {
        return false;
    }

    const float Speed = GetKickSpeed(KickType, Charge);
    if (Speed <= 0.0f)
    {
        return false;
    }

    float VerticalLift = 0.0f;
    float Spin = 0.0f;

    switch (KickType)
    {
    case EFootballKickType::LongPass:
        VerticalLift = 0.20f;
        break;
    case EFootballKickType::ThroughPass:
        VerticalLift = 0.04f;
        break;
    case EFootballKickType::Cross:
        VerticalLift = 0.35f;
        Spin = 8.0f;
        break;
    case EFootballKickType::NormalShot:
        Spin = 2.5f;
        break;
    case EFootballKickType::PowerShot:
        Spin = 1.0f;
        break;
    default:
        break;
    }

    const FVector KickDirection = (SafeDirection + FVector::UpVector * VerticalLift).GetSafeNormal();
    Ball->Kick(KickDirection, Speed, Spin);
    return true;
}
