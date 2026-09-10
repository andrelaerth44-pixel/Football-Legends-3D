#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Match/FootballMatchTypes.h"
#include "FootballGoalPresentationComponent.generated.h"

class AFootballBall;
class UFootballGoalSequenceComponent;
class UFootballGoalReplayComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
    FFootballGoalPresentationSignature,
    EFootballTeamSide, ScoringSide,
    int32, HomeScore,
    int32, AwayScore,
    AFootballBall*, Ball,
    float, ImpactSpeed);

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballGoalPresentationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballGoalPresentationComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Presentation")
    bool bAutoStartReplay = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Presentation", meta=(ClampMin="0.0"))
    float ReplayDelay = 0.25f;

    UPROPERTY(BlueprintAssignable, Category="Goal Presentation")
    FFootballGoalPresentationSignature OnPresentationStarted;

    UPROPERTY(BlueprintAssignable, Category="Goal Presentation")
    FFootballGoalPresentationSignature OnScoreboardShown;

    UFUNCTION(BlueprintCallable, Category="Goal Presentation")
    void PlayGoalPresentation(EFootballTeamSide ScoringSide, int32 HomeScore, int32 AwayScore, AFootballBall* Ball, float ImpactSpeed);

    UFUNCTION(BlueprintCallable, Category="Goal Presentation")
    void StartReplay();

    UFUNCTION(BlueprintCallable, Category="Goal Presentation")
    void StopReplay();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UFUNCTION()
    void HandleGoalSequenceStarted(EFootballTeamSide ScoringSide, int32 HomeScore, int32 AwayScore, AFootballBall* Ball, float ImpactSpeed, float MatchTimeSeconds);

    UFUNCTION()
    void HandleGoalSequenceFinished();

    void StartReplayIfReady();

    UPROPERTY()
    TObjectPtr<UFootballGoalSequenceComponent> Sequence;

    UPROPERTY()
    TObjectPtr<UFootballGoalReplayComponent> Replay;

    EFootballTeamSide PendingScoringSide = EFootballTeamSide::Home;
    int32 PendingHomeScore = 0;
    int32 PendingAwayScore = 0;
    float PendingImpactSpeed = 0.0f;
    float ReplayDelayRemaining = 0.0f;
    bool bPendingReplay = false;
    bool bPresentationActive = false;
};
