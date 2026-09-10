#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballGoalNetDeformationComponent.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;

/** Drives a short-lived visual net reaction when the ball enters the goal. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballGoalNetDeformationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballGoalNetDeformationComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Net", meta=(ClampMin="0.0"))
    float ReactionStrength = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal Net", meta=(ClampMin="0.01"))
    float ReactionDuration = 0.30f;

    UFUNCTION(BlueprintCallable, Category="Goal Net")
    void ReactToGoal(const FVector& ImpactDirection, float ImpactSpeed);

    UFUNCTION(BlueprintPure, Category="Goal Net")
    bool IsReacting() const;

protected:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float ReactionTimeRemaining = 0.0f;
    FVector ReactionOffset = FVector::ZeroVector;
    FVector RestRelativeLocation = FVector::ZeroVector;
};
