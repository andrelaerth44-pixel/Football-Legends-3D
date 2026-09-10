#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FootballMatchState.generated.h"

class UFootballMatchScoreComponent;
class UFootballGoalSequenceComponent;
class UFootballGoalReplayComponent;
class UFootballGoalReplayDirectorComponent;
class UFootballGoalPresentationComponent;

UCLASS()
class FOOTBALLLEGENDS3D_API AFootballMatchState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AFootballMatchState();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match")
    TObjectPtr<UFootballMatchScoreComponent> Score;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match|Goal")
    TObjectPtr<UFootballGoalSequenceComponent> GoalSequence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match|Goal")
    TObjectPtr<UFootballGoalReplayComponent> GoalReplay;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match|Goal")
    TObjectPtr<UFootballGoalReplayDirectorComponent> GoalReplayDirector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match|Goal")
    TObjectPtr<UFootballGoalPresentationComponent> GoalPresentation;
};
