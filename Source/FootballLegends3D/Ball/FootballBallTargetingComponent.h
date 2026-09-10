#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballBallTargetingComponent.generated.h"

class AFootballPlayer;

/** Finds the best nearby teammate or forward target for contextual passes. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballBallTargetingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballBallTargetingComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
    float SearchRadius = 2200.0f;

    UFUNCTION(BlueprintCallable, Category="Targeting")
    AFootballPlayer* FindBestPassTarget() const;

private:
    float ScoreTarget(const AFootballPlayer* Candidate) const;
};
