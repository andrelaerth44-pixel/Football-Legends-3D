#include "Match/FootballGoalSequenceComponent.h"
#include "Match/FootballMatchScoreComponent.h"
#include "Match/FootballKickoffComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UFootballGoalSequenceComponent::UFootballGoalSequenceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalSequenceComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        Score = Owner->FindComponentByClass<UFootballMatchScoreComponent>();
        Kickoff = Owner->FindComponentByClass<UFootballKickoffComponent>();
    }

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
    bKickoffRequested = false;
    SequenceTimeRemaining = FMath::Max(0.0f, GoalFreezeDuration + PresentationDuration);
    KickoffRequestTime = FMath::Max(0.0f, PresentationDuration);

    // The team that conceded the goal gets the restart.
    if (Kickoff)
    {
        const EFootballTeamSide RestartSide =
            ScoringSide == EFootballTeamSide::Home ? EFootballTeamSide::Away : EFootballTeamSide::Home;
        Kickoff->SetKickoffSide(RestartSide);
    }

    Score->PauseMatchClock();

    OnGoalSequenceStarted.Broadcast(
        ScoringSide,
        HomeScore,
        AwayScore,
        Ball,
        ImpactSpeed,
        Score->MatchTimeSeconds);
}

void UFootballGoalSequenceComponent::RequestKickoff()
{
    if (bKickoffRequested)
    {
        return;
    }

    bKickoffRequested = true;

    if (Kickoff)
    {
        Kickoff->PrepareKickoff();
        Kickoff->StartKickoff();
    }

    OnKickoffRequested.Broadcast();
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

    if (!bKickoffRequested)
    {
        KickoffRequestTime = FMath::Max(0.0f, KickoffRequestTime - SafeDelta);
        if (KickoffRequestTime <= KINDA_SMALL_NUMBER)
        {
            RequestKickoff();
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

    // In case the presentation was shortened/disabled, make sure kickoff is
    // never forgotten before gameplay resumes.
    if (!bKickoffRequested)
    {
        RequestKickoff();
    }

    if (Score && !Score->IsMatchFinished())
    {
        Score->StartMatchClock();
    }

    OnGoalSequenceFinished.Broadcast();
}
