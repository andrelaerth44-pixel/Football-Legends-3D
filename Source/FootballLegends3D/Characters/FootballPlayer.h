#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FootballPlayer.generated.h"

class UPlayerAttributeComponent;
class UPlayerAppearanceComponent;
class UFootballBallInteractionComponent;

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
};
