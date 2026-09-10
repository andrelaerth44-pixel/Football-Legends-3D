#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match/FootballMatchTypes.h"
#include "FootballGoalActor.generated.h"

class UBoxComponent;
class USceneComponent;
class UPrimitiveComponent;
class UFootballGoalNetDeformationComponent;
class AFootballBall;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFootballGoalScoredSignature, AFootballBall*, Ball, float, ImpactSpeed, EFootballTeamSide, ScoringSide);

/** Goal volume that detects a ball entering the goal and emits a goal event. */
UCLASS()
class FOOTBALLLEGENDS3D_API AFootballGoalActor : public AActor
{
    GENERATED_BODY()

public:
    AFootballGoalActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Goal")
    TObjectPtr<UBoxComponent> GoalVolume;

    /** Child scene root intended for the actual net mesh/cloth. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Goal")
    TObjectPtr<USceneComponent> NetVisualRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Goal")
    TObjectPtr<UFootballGoalNetDeformationComponent> NetReaction;

    /** Team awarded the goal when this goal volume is crossed. */
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Goal")
    EFootballTeamSide ScoringSide = EFootballTeamSide::Home;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal", meta=(ClampMin="0.05"))
    float GoalCooldown = 1.0f;

    UPROPERTY(BlueprintAssignable, Category="Goal")
    FFootballGoalScoredSignature OnGoalScored;

    UFUNCTION(BlueprintCallable, Category="Goal")
    void ResetGoal();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnGoalVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    float LastGoalTime = -1000.0f;
};
