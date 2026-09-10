#include "Characters/FootballPlayer.h"
#include "Components/PlayerAttributeComponent.h"
#include "Components/PlayerAppearanceComponent.h"
#include "Components/FootballBallInteractionComponent.h"
#include "Characters/FootballPlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AFootballPlayer::AFootballPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UPlayerAttributeComponent>(TEXT("Attributes"));
    Appearance = CreateDefaultSubobject<UPlayerAppearanceComponent>(TEXT("Appearance"));
    BallInteraction = CreateDefaultSubobject<UFootballBallInteractionComponent>(TEXT("BallInteraction"));
    FootballMovement = CreateDefaultSubobject<UFootballPlayerMovementComponent>(TEXT("FootballMovement"));

    GetCharacterMovement()->MaxWalkSpeed = 420.0f;
    GetCharacterMovement()->MaxAcceleration = 1800.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1600.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}
