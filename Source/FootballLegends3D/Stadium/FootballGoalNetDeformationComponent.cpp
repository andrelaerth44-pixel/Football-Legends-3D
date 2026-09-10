#include "Stadium/FootballGoalNetDeformationComponent.h"
#include "Components/SceneComponent.h"

UFootballGoalNetDeformationComponent::UFootballGoalNetDeformationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalNetDeformationComponent::SetTargetComponent(USceneComponent* InTargetComponent)
{
    TargetComponent = InTargetComponent;
    if (TargetComponent)
    {
        RestRelativeLocation = TargetComponent->GetRelativeLocation();
    }
}

void UFootballGoalNetDeformationComponent::ReactToGoal(const FVector& ImpactDirection, float ImpactSpeed)
{
    if (!TargetComponent)
    {
        return;
    }

    const float Intensity = FMath::Clamp(ImpactSpeed / 2500.0f, 0.25f, 1.0f);
    const FVector Direction = ImpactDirection.GetSafeNormal();
    ReactionOffset = -Direction * ReactionStrength * Intensity;
    ReactionTimeRemaining = ReactionDuration;
    RestRelativeLocation = TargetComponent->GetRelativeLocation();
}

bool UFootballGoalNetDeformationComponent::IsReacting() const
{
    return ReactionTimeRemaining > 0.0f;
}

void UFootballGoalNetDeformationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!TargetComponent || ReactionTimeRemaining <= 0.0f)
    {
        return;
    }

    ReactionTimeRemaining = FMath::Max(0.0f, ReactionTimeRemaining - DeltaTime);
    const float Alpha = ReactionDuration > 0.0f ? ReactionTimeRemaining / ReactionDuration : 0.0f;
    const float Wave = FMath::Sin((1.0f - Alpha) * PI);

    TargetComponent->SetRelativeLocation(RestRelativeLocation + ReactionOffset * Wave);
    if (ReactionTimeRemaining <= KINDA_SMALL_NUMBER)
    {
        TargetComponent->SetRelativeLocation(RestRelativeLocation);
    }
}
