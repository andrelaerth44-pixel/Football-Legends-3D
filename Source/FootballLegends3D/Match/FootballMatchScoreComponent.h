#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballMatchScoreComponent.generated.h"

class AFootballBall;
class AFootballGoalActor;

enum class EFootballTeamSide : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFootballGoalEventSignature, EFootballTeamSide, ScoringSide, int32, HomeScore, int32, AwayScore, AFootballBall*, Ball);

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
    void RegisterGoal(EFootballTeamSide ScoringSide, AFootballBall* Ball);

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
    void HandleHomeGoal(AFootballBall* Ball, float ImpactSpeed);

    UFUNCTION()
    void HandleAwayGoal(AFootballBall* Ball, float ImpactSpeed);

    UPROPERTY(EditInstanceOnly, Category="Goals")
    TObjectPtr<AFootballGoalActor> HomeGoal;

    UPROPERTY(EditInstanceOnly, Category="Goals")
    TObjectPtr<AFootballGoalActor> AwayGoal;
};
