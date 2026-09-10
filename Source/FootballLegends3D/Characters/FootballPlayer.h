#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FootballPlayer.generated.h"

class UPlayerAttributeComponent;
class UPlayerAppearanceComponent;
class UFootballBallInteractionComponent;
class UFootballPlayerMovementComponent;
class UFootballBallGameplayComponent;
class UFootballBallPossessionComponent;
class UFootballBallTargetingComponent;

/** Reusable base character for every football player in the game. */
UCLASS()
class FOOTBALLLEGENDS3D_API AFootballPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    AFootballPlayer();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    TObjectPtr<UPlayerAttributeComponent> Attributes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    TObjectPtr<UPlayerAppearanceComponent> Appearance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Ball")
    TObjectPtr<UFootballBallInteractionComponent> BallInteraction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Ball")
    TObjectPtr<UFootballBallGameplayComponent> BallGameplay;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Ball")
    TObjectPtr<UFootballBallPossessionComponent> BallPossession;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Ball")
    TObjectPtr<UFootballBallTargetingComponent> BallTargeting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement")
    TObjectPtr<UFootballPlayerMovementComponent> FootballMovement;
};
