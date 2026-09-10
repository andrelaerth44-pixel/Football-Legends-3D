#include "Match/FootballMatchState.h"
#include "Match/FootballMatchScoreComponent.h"
#include "Match/FootballGoalSequenceComponent.h"
#include "Match/FootballGoalReplayComponent.h"
#include "Match/FootballGoalReplayDirectorComponent.h"
#include "Match/FootballGoalPresentationComponent.h"

AFootballMatchState::AFootballMatchState()
{
    Score = CreateDefaultSubobject<UFootballMatchScoreComponent>(TEXT("Score"));
    GoalSequence = CreateDefaultSubobject<UFootballGoalSequenceComponent>(TEXT("GoalSequence"));
    GoalReplay = CreateDefaultSubobject<UFootballGoalReplayComponent>(TEXT("GoalReplay"));
    GoalReplayDirector = CreateDefaultSubobject<UFootballGoalReplayDirectorComponent>(TEXT("GoalReplayDirector"));
    GoalPresentation = CreateDefaultSubobject<UFootballGoalPresentationComponent>(TEXT("GoalPresentation"));
}
