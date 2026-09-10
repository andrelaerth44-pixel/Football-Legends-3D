#include "Match/FootballGoalPresentationComponent.h"
#include "Match/FootballGoalSequenceComponent.h"
#include "Match/FootballGoalReplayComponent.h"
#include "Match/FootballGoalReplayDirectorComponent.h"
#include "GameFramework/Actor.h"

UFootballGoalPresentationComponent::UFootballGoalPresentationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalPresentationComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        Sequence = Owner->FindComponentByClass<UFootballGoalSequenceComponent>();
        Replay = Owner->FindComponentByClass<UFootballGoalReplayComponent>();
        ReplayDirector = Owner->FindComponentByClass<UFootballGoalReplayDirectorComponent>();
    }

    if (Sequence)
    {
        Sequence->OnGoalSequenceStarted.AddDynamic(this, &UFootballGoalPresentationComponent::HandleGoalSequenceStarted);
        Sequence->OnGoalSequenceFinished.AddDynamic(this, &UFootballGoalPresentationComponent::HandleGoalSequenceFinished);
    }
}

void UFootballGoalPresentationComponent::HandleGoalSequenceStarted(
    EFootballTeamSide ScoringSide,
    int32 HomeScore,
    int32 AwayScore,
    AFootballBall* Ball,
    float ImpactSpeed,
    float MatchTimeSeconds)
{
    PlayGoalPresentation(ScoringSide, HomeScore, AwayScore, Ball, ImpactSpeed);
}

void UFootballGoalPresentationComponent::PlayGoalPresentation(
    EFootballTeamSide ScoringSide,
    int32 HomeScore,
    int32 AwayScore,
    AFootballBall* Ball,
    float ImpactSpeed)
{
    PendingScoringSide = ScoringSide;
    PendingHomeScore = HomeScore;
    PendingAwayScore = AwayScore;
    PendingImpactSpeed = ImpactSpeed;
    bPresentationActive = true;
    bPendingReplay = bAutoStartReplay && IsValid(Ball);
    ReplayDelayRemaining = FMath::Max(0.0f, ReplayDelay);

    // Keep the recorder's rolling buffer intact. It is already tracking the
    // ball from match start, so the replay contains the pre-goal approach.
    if (Replay && Ball)
    {
        Replay->SetTrackedBall(Ball);
    }

    OnPresentationStarted.Broadcast(ScoringSide, HomeScore, AwayScore, Ball, ImpactSpeed);
    OnScoreboardShown.Broadcast(ScoringSide, HomeScore, AwayScore, Ball, ImpactSpeed);

    if (ReplayDelayRemaining <= KINDA_SMALL_NUMBER)
    {
        StartReplayIfReady();
    }
}

void UFootballGoalPresentationComponent::StartReplayIfReady()
{
    if (!bPendingReplay || !Replay)
    {
        return;
    }

    bPendingReplay = false;
    Replay->StartReplay();

    if (bAutoStartReplayCamera && ReplayDirector && Replay->IsReplaying())
    {
        ReplayDirector->StartGoalReplay(Replay->GetTrackedBall());
    }
}

void UFootballGoalPresentationComponent::StartReplay()
{
    if (Replay)
    {
        Replay->StartReplay();
        if (bAutoStartReplayCamera && ReplayDirector && Replay->IsReplaying())
        {
            ReplayDirector->StartGoalReplay(Replay->GetTrackedBall());
        }
    }
}

void UFootballGoalPresentationComponent::StopReplay()
{
    if (ReplayDirector)
    {
        ReplayDirector->StopGoalReplay();
    }
    else if (Replay)
    {
        Replay->StopReplay();
    }
}

void UFootballGoalPresentationComponent::HandleGoalSequenceFinished()
{
    bPresentationActive = false;
    bPendingReplay = false;
    ReplayDelayRemaining = 0.0f;
    StopReplay();
}

void UFootballGoalPresentationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bPresentationActive || !bPendingReplay)
    {
        return;
    }

    ReplayDelayRemaining = FMath::Max(0.0f, ReplayDelayRemaining - FMath::Max(0.0f, DeltaTime));
    if (ReplayDelayRemaining <= KINDA_SMALL_NUMBER)
    {
        StartReplayIfReady();
    }
}
