#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballGoalReplayComponent.generated.h"

class AFootballBall;

USTRUCT(BlueprintType)
struct FFootballReplayFrame
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float Time = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FVector BallLocation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    FRotator BallRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadOnly)
    FVector BallVelocity = FVector::ZeroVector;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFootballReplaySimpleSignature);

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballGoalReplayComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballGoalReplayComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay", meta=(ClampMin="1.0"))
    float BufferDuration = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay", meta=(ClampMin="0.01"))
    float SampleInterval = 0.033f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Replay", meta=(ClampMin="0.1"))
    float ReplaySpeed = 0.75f;

    UPROPERTY(BlueprintAssignable, Category="Goal Replay")
    FFootballReplaySimpleSignature OnReplayStarted;

    UPROPERTY(BlueprintAssignable, Category="Goal Replay")
    FFootballReplaySimpleSignature OnReplayFinished;

    UFUNCTION(BlueprintCallable, Category="Goal Replay")
    void SetTrackedBall(AFootballBall* Ball);

    UFUNCTION(BlueprintCallable, Category="Goal Replay")
    void StartReplay();

    UFUNCTION(BlueprintCallable, Category="Goal Replay")
    void StopReplay();

    UFUNCTION(BlueprintPure, Category="Goal Replay")
    bool IsReplaying() const;

    UFUNCTION(BlueprintPure, Category="Goal Replay")
    float GetReplayProgress() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void CaptureFrame();
    void UpdateReplay(float DeltaTime);
    void ApplyFrame(const FFootballReplayFrame& Frame);

    UPROPERTY()
    TObjectPtr<AFootballBall> TrackedBall;

    UPROPERTY()
    TArray<FFootballReplayFrame> Frames;

    float SampleAccumulator = 0.0f;
    float ReplayTime = 0.0f;
    float ReplayDuration = 0.0f;
    bool bReplaying = false;
};
