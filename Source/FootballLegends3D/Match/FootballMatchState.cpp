#include "Match/FootballMatchState.h"
#include "Match/FootballMatchScoreComponent.h"
#include "Match/FootballGoalSequenceComponent.h"

AFootballMatchState::AFootballMatchState()
{
    Score = CreateDefaultSubobject<UFootballMatchScoreComponent>(TEXT("Score"));
    GoalSequence = CreateDefaultSubobject<UFootballGoalSequenceComponent>(TEXT("GoalSequence"));
}
