#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Match/FootballMatchTypes.h"
#include "FootballGoalSequenceComponent.generated.h"

class AFootballBall;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
    FFootballGoalSequenceStartedSignature,
    EFootballTeamSide, ScoringSide,
    int32, HomeScore,
    int32, AwayScore,
    AFootballBall*, Ball,
    float, ImpactSpeed,
    float, MatchTimeSeconds);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFootballGoalSequenceSimpleSignature);

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballGoalSequenceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballGoalSequenceComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Sequence", meta=(ClampMin="0.0"))
    float GoalFreezeDuration = 0.20f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Sequence", meta=(ClampMin="0.0"))
    float PresentationDuration = 2.0f;

    UPROPERTY(BlueprintAssignable, Category="Goal Sequence")
    FFootballGoalSequenceStartedSignature OnGoalSequenceStarted;

    UPROPERTY(BlueprintAssignable, Category="Goal Sequence")
    FFootballGoalSequenceSimpleSignature OnKickoffRequested;

    UPROPERTY(BlueprintAssignable, Category="Goal Sequence")
    FFootballGoalSequenceSimpleSignature OnGoalSequenceFinished;

    UFUNCTION(BlueprintPure, Category="Goal Sequence")
    bool IsGoalSequenceActive() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UFUNCTION()
    void HandleGoal(EFootballTeamSide ScoringSide, int32 HomeScore, int32 AwayScore, AFootballBall* Ball, float ImpactSpeed);

    void FinishSequence();

    UPROPERTY()
    TObjectPtr<class UFootballMatchScoreComponent> Score;

    float SequenceTimeRemaining = 0.0f;
    float KickoffRequestTime = 0.0f;
    bool bSequenceActive = false;
};
