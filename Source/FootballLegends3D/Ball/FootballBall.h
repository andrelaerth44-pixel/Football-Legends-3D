#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FootballBall.generated.h"

class UStaticMeshComponent;

/** Physical football with short-lived visual deformation on powerful impacts. */
UCLASS()
class FOOTBALLLEGENDS3D_API AFootballBall : public AActor
{
    GENERATED_BODY()

public:
    AFootballBall();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ball")
    TObjectPtr<UStaticMeshComponent> BallMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Physics", meta=(ClampMin="0.01"))
    float BallMass = 0.43f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Physics", meta=(ClampMin="0.0"))
    float RollingFriction = 0.18f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Physics", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Restitution = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Visual", meta=(ClampMin="0.0", ClampMax="0.35"))
    float MaxDeformation = 0.14f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Visual", meta=(ClampMin="0.01"))
    float DeformationDuration = 0.10f;

    UFUNCTION(BlueprintCallable, Category="Ball")
    void Kick(const FVector& Direction, float Speed, float Spin = 0.0f);

    UFUNCTION(BlueprintCallable, Category="Ball")
    void StopBall();

    UFUNCTION(BlueprintPure, Category="Ball")
    bool IsMoving() const;

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    FVector RestScale = FVector::OneVector;
    float DeformationTimeRemaining = 0.0f;
    FVector DeformationAxis = FVector::ForwardVector;

    void TriggerDeformation(const FVector& Direction, float Speed);
    void UpdateDeformation(float DeltaSeconds);
};
