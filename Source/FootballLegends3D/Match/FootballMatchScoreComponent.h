#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Match/FootballMatchTypes.h"
#include "FootballMatchScoreComponent.generated.h"

class AFootballBall;
class AFootballGoalActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FFootballGoalEventSignature, EFootballTeamSide, ScoringSide, int32, HomeScore, int32, AwayScore, AFootballBall*, Ball, float, ImpactSpeed);

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballMatchScoreComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballMatchScoreComponent();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
    int32 HomeScore = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Score")
    int32 AwayScore = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match")
    float MatchTimeSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Match", meta=(ClampMin="1.0"))
    float MatchDurationSeconds = 5400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Match")
    bool bMatchClockRunning = true;

    UPROPERTY(BlueprintAssignable, Category="Score")
    FFootballGoalEventSignature OnGoal;

    UFUNCTION(BlueprintCallable, Category="Score")
    void RegisterGoal(EFootballTeamSide ScoringSide, AFootballBall* Ball, float ImpactSpeed = 0.0f);

    UFUNCTION(BlueprintCallable, Category="Score")
    void ResetScore();

    UFUNCTION(BlueprintCallable, Category="Match")
    void StartMatchClock();

    UFUNCTION(BlueprintCallable, Category="Match")
    void PauseMatchClock();

    UFUNCTION(BlueprintPure, Category="Match")
    bool IsMatchFinished() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UFUNCTION()
    void HandleGoal(AFootballBall* Ball, float ImpactSpeed, EFootballTeamSide ScoringSide);

    UPROPERTY()
    TArray<TObjectPtr<AFootballGoalActor>> RegisteredGoals;
};
