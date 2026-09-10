#include "Match/FootballMatchScoreComponent.h"
#include "Stadium/FootballGoalActor.h"
#include "Ball/FootballBall.h"

UFootballMatchScoreComponent::UFootballMatchScoreComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballMatchScoreComponent::BeginPlay()
{
    Super::BeginPlay();

    if (HomeGoal)
    {
        HomeGoal->OnGoalScored.AddDynamic(this, &UFootballMatchScoreComponent::HandleHomeGoal);
    }

    if (AwayGoal)
    {
        AwayGoal->OnGoalScored.AddDynamic(this, &UFootballMatchScoreComponent::HandleAwayGoal);
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

void UFootballMatchScoreComponent::RegisterGoal(EFootballTeamSide ScoringSide, AFootballBall* Ball)
{
    if (IsMatchFinished())
    {
        return;
    }

    if (ScoringSide == EFootballTeamSide::Home)
    {
        ++HomeScore;
    }
    else
    {
        ++AwayScore;
    }

    OnGoal.Broadcast(ScoringSide, HomeScore, AwayScore, Ball);
}

void UFootballMatchScoreComponent::HandleHomeGoal(AFootballBall* Ball, float ImpactSpeed)
{
    RegisterGoal(EFootballTeamSide::Home, Ball);
}

void UFootballMatchScoreComponent::HandleAwayGoal(AFootballBall* Ball, float ImpactSpeed)
{
    RegisterGoal(EFootballTeamSide::Away, Ball);
}

void UFootballMatchScoreComponent::ResetScore()
{
    HomeScore = 0;
    AwayScore = 0;
    MatchTimeSeconds = 0.0f;
    bMatchClockRunning = true;

    if (HomeGoal)
    {
        HomeGoal->ResetGoal();
    }

    if (AwayGoal)
    {
        AwayGoal->ResetGoal();
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
