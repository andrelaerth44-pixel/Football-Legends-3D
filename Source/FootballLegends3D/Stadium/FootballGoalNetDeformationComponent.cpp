#include "Stadium/FootballGoalNetDeformationComponent.h"
#include "Components/SceneComponent.h"

UFootballGoalNetDeformationComponent::UFootballGoalNetDeformationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalNetDeformationComponent::BeginPlay()
{
    Super::BeginPlay();
    CacheRestLocation();
}

void UFootballGoalNetDeformationComponent::SetTargetComponent(USceneComponent* InTargetComponent)
{
    TargetComponent = InTargetComponent;
    CacheRestLocation();
}

void UFootballGoalNetDeformationComponent::CacheRestLocation()
{
    RestRelativeLocation = TargetComponent
        ? TargetComponent->GetRelativeLocation()
        : FVector::ZeroVector;
}

void UFootballGoalNetDeformationComponent::ReactToGoal(const FVector& ImpactDirection, float ImpactSpeed)
{
    if (!TargetComponent)
    {
        return;
    }

    CacheRestLocation();
    const float Intensity = FMath::Clamp(ImpactSpeed / 2500.0f, 0.25f, 1.0f);
    const FVector Direction = ImpactDirection.GetSafeNormal();
    ReactionOffset = -Direction * ReactionStrength * Intensity;
    ReactionTimeRemaining = ReactionDuration;
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
    const float Alpha = ReactionDuration > 0.0f
        ? 1.0f - (ReactionTimeRemaining / ReactionDuration)
        : 1.0f;
    const float Wave = FMath::Sin(Alpha * PI);

    TargetComponent->SetRelativeLocation(RestRelativeLocation + ReactionOffset * Wave);

    if (ReactionTimeRemaining <= KINDA_SMALL_NUMBER)
    {
        TargetComponent->SetRelativeLocation(RestRelativeLocation);
    }
}
