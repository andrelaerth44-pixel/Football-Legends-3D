#include "Match/FootballMatchState.h"
#include "Match/FootballMatchScoreComponent.h"

AFootballMatchState::AFootballMatchState()
{
    Score = CreateDefaultSubobject<UFootballMatchScoreComponent>(TEXT("Score"));
}
