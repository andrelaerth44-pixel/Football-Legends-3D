#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballGoalReplayDirectorComponent.generated.h"

class AFootballBall;
class AFootballGoalReplayCameraActor;
class UFootballGoalReplayComponent;

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballGoalReplayDirectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballGoalReplayDirectorComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Replay Director")
    bool bAutoPlayCamera = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Replay Director")
    float CameraBlendTime = 0.35f;

    UFUNCTION(BlueprintCallable, Category="Replay Director")
    void StartGoalReplay(AFootballBall* Ball);

    UFUNCTION(BlueprintCallable, Category="Replay Director")
    void StopGoalReplay();

    UFUNCTION(BlueprintPure, Category="Replay Director")
    bool IsGoalReplayActive() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UFootballGoalReplayComponent> Replay;

    UPROPERTY()
    TObjectPtr<AFootballGoalReplayCameraActor> ReplayCamera;

    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;

    TWeakObjectPtr<AActor> PreviousViewTarget;
};
