#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FootballMatchState.generated.h"

class UFootballMatchScoreComponent;

UCLASS()
class FOOTBALLLEGENDS3D_API AFootballMatchState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AFootballMatchState();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match")
    TObjectPtr<UFootballMatchScoreComponent> Score;
};
