#include "Match/FootballMatchScoreComponent.h"
#include "Stadium/FootballGoalActor.h"
#include "Ball/FootballBall.h"
#include "Characters/FootballPlayer.h"
#include "Teams/FootballTeamComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"

UFootballMatchScoreComponent::UFootballMatchScoreComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballMatchScoreComponent::BeginPlay()
{
    Super::BeginPlay();

    RegisteredGoals.Reset();

    if (!GetWorld())
    {
        return;
    }

    for (TActorIterator<AFootballGoalActor> It(GetWorld()); It; ++It)
    {
        AFootballGoalActor* Goal = *It;
        if (!IsValid(Goal))
        {
            continue;
        }

        RegisteredGoals.Add(Goal);
        Goal->OnGoalScored.AddDynamic(this, &UFootballMatchScoreComponent::HandleGoal);
    }
}

void UFootballMatchScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bMatchClockRunning || IsMatchFinished())
    {
        return;
    }

    MatchTimeSeconds = FMath::Min(MatchDurationSeconds, MatchTimeSeconds + DeltaTime);

    if (MatchTimeSeconds >= MatchDurationSeconds)
    {
        bMatchClockRunning = false;
    }
}

void UFootballMatchScoreComponent::RegisterGoal(EFootballTeamSide ScoringSide, AFootballBall* Ball, float ImpactSpeed)
{
    if (IsMatchFinished())
    {
        return;
    }

    LastScorer = nullptr;
    LastAssist = nullptr;

    if (IsValid(Ball))
    {
        LastScorer = Ball->GetLastKicker();
        if (!IsValid(LastScorer))
        {
            LastScorer = Ball->GetLastTouchPlayer();
        }

        AFootballPlayer* CandidateAssist = Ball->GetPreviousKicker();
        if (IsValid(LastScorer) && IsValid(CandidateAssist) && CandidateAssist != LastScorer
            && LastScorer->Team && CandidateAssist->Team
            && LastScorer->Team->Side == CandidateAssist->Team->Side)
        {
            LastAssist = CandidateAssist;
        }
    }

    if (ScoringSide == EFootballTeamSide::Home)
    {
        ++HomeScore;
    }
    else
    {
        ++AwayScore;
    }

    OnGoal.Broadcast(ScoringSide, HomeScore, AwayScore, Ball, ImpactSpeed);
}

void UFootballMatchScoreComponent::HandleGoal(AFootballBall* Ball, float ImpactSpeed, EFootballTeamSide ScoringSide)
{
    RegisterGoal(ScoringSide, Ball, ImpactSpeed);
}

void UFootballMatchScoreComponent::ResetScore()
{
    HomeScore = 0;
    AwayScore = 0;
    LastScorer = nullptr;
    LastAssist = nullptr;
    MatchTimeSeconds = 0.0f;
    bMatchClockRunning = true;

    for (AFootballGoalActor* Goal : RegisteredGoals)
    {
        if (IsValid(Goal))
        {
            Goal->ResetGoal();
        }
    }
}

void UFootballMatchScoreComponent::StartMatchClock()
{
    if (!IsMatchFinished())
    {
        bMatchClockRunning = true;
    }
}

void UFootballMatchScoreComponent::PauseMatchClock()
{
    bMatchClockRunning = false;
}

bool UFootballMatchScoreComponent::IsMatchFinished() const
{
    return MatchTimeSeconds >= MatchDurationSeconds;
}
