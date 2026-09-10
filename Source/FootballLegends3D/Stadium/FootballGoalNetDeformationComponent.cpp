#include "Stadium/FootballGoalNetDeformationComponent.h"
#include "GameFramework/Actor.h"

UFootballGoalNetDeformationComponent::UFootballGoalNetDeformationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalNetDeformationComponent::ReactToGoal(const FVector& ImpactDirection, float ImpactSpeed)
{
    if (!GetOwner())
    {
        return;
    }

    const float Intensity = FMath::Clamp(ImpactSpeed / 2500.0f, 0.25f, 1.0f);
    const FVector Direction = ImpactDirection.GetSafeNormal();
    ReactionOffset = -Direction * ReactionStrength * Intensity;
    ReactionTimeRemaining = ReactionDuration;

    RestRelativeLocation = GetOwner()->GetRootComponent()
        ? GetOwner()->GetRootComponent()->GetRelativeLocation()
        : FVector::ZeroVector;
}

bool UFootballGoalNetDeformationComponent::IsReacting() const
{
    return ReactionTimeRemaining > 0.0f;
}

void UFootballGoalNetDeformationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!GetOwner() || ReactionTimeRemaining <= 0.0f)
    {
        return;
    }

    ReactionTimeRemaining = FMath::Max(0.0f, ReactionTimeRemaining - DeltaTime);
    const float Alpha = ReactionDuration > 0.0f ? ReactionTimeRemaining / ReactionDuration : 0.0f;
    const float Wave = FMath::Sin((1.0f - Alpha) * PI);

    if (USceneComponent* Root = GetOwner()->GetRootComponent())
    {
        Root->SetRelativeLocation(RestRelativeLocation + ReactionOffset * Wave);
        if (ReactionTimeRemaining <= KINDA_SMALL_NUMBER)
        {
            Root->SetRelativeLocation(RestRelativeLocation);
        }
    }
}
