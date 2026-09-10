#include "Match/FootballGoalSequenceComponent.h"
#include "Match/FootballMatchScoreComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UFootballGoalSequenceComponent::UFootballGoalSequenceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalSequenceComponent::BeginPlay()
{
    Super::BeginPlay();

    Score = GetOwner() ? GetOwner()->FindComponentByClass<UFootballMatchScoreComponent>() : nullptr;
    if (Score)
    {
        Score->OnGoal.AddDynamic(this, &UFootballGoalSequenceComponent::HandleGoal);
    }
}

bool UFootballGoalSequenceComponent::IsGoalSequenceActive() const
{
    return bSequenceActive;
}

void UFootballGoalSequenceComponent::HandleGoal(
    EFootballTeamSide ScoringSide,
    int32 HomeScore,
    int32 AwayScore,
    AFootballBall* Ball,
    float ImpactSpeed)
{
    if (bSequenceActive || !Score)
    {
        return;
    }

    bSequenceActive = true;
    SequenceTimeRemaining = FMath::Max(0.0f, GoalFreezeDuration + PresentationDuration);
    KickoffRequestTime = FMath::Max(0.0f, PresentationDuration);

    // Freeze only the match clock. This keeps UI, camera and presentation
    // systems responsive while gameplay time is stopped safely.
    Score->PauseMatchClock();

    OnGoalSequenceStarted.Broadcast(
        ScoringSide,
        HomeScore,
        AwayScore,
        Ball,
        ImpactSpeed,
        Score->MatchTimeSeconds);
}

void UFootballGoalSequenceComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bSequenceActive)
    {
        return;
    }

    const float SafeDelta = FMath::Max(0.0f, DeltaTime);
    SequenceTimeRemaining = FMath::Max(0.0f, SequenceTimeRemaining - SafeDelta);

    if (KickoffRequestTime > 0.0f)
    {
        KickoffRequestTime = FMath::Max(0.0f, KickoffRequestTime - SafeDelta);
        if (KickoffRequestTime <= KINDA_SMALL_NUMBER)
        {
            OnKickoffRequested.Broadcast();
        }
    }

    if (SequenceTimeRemaining <= KINDA_SMALL_NUMBER)
    {
        FinishSequence();
    }
}

void UFootballGoalSequenceComponent::FinishSequence()
{
    if (!bSequenceActive)
    {
        return;
    }

    bSequenceActive = false;
    SequenceTimeRemaining = 0.0f;
    KickoffRequestTime = 0.0f;

    if (Score && !Score->IsMatchFinished())
    {
        Score->StartMatchClock();
    }

    OnGoalSequenceFinished.Broadcast();
}
