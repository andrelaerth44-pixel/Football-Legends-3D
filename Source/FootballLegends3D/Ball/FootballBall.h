#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FootballBall.generated.h"

class UStaticMeshComponent;

/** Physical football actor shared by gameplay, AI and ball-control systems. */
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

    UFUNCTION(BlueprintCallable, Category="Ball")
    void Kick(const FVector& Direction, float Speed, float Spin = 0.0f);

    UFUNCTION(BlueprintCallable, Category="Ball")
    void StopBall();

    UFUNCTION(BlueprintPure, Category="Ball")
    bool IsMoving() const;
};
