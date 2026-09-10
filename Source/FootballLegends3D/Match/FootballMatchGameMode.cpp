#include "Match/FootballMatchGameMode.h"
#include "Match/FootballMatchState.h"

AFootballMatchGameMode::AFootballMatchGameMode()
{
    GameStateClass = AFootballMatchState::StaticClass();
}
