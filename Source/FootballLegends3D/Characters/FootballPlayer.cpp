#include "Characters/FootballPlayer.h"
#include "Components/PlayerAttributeComponent.h"
#include "Components/PlayerAppearanceComponent.h"
#include "Components/FootballBallInteractionComponent.h"

AFootballPlayer::AFootballPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UPlayerAttributeComponent>(TEXT("Attributes"));
    Appearance = CreateDefaultSubobject<UPlayerAppearanceComponent>(TEXT("Appearance"));
    BallInteraction = CreateDefaultSubobject<UFootballBallInteractionComponent>(TEXT("BallInteraction"));
}
