#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballBallGameplayComponent.generated.h"

class AFootballBall;

UENUM(BlueprintType)
enum class EFootballKickType : uint8
{
    ShortPass,
    LongPass,
    ThroughPass,
    Cross,
    NormalShot,
    PowerShot
};

/** Gameplay layer for deterministic football passes and shots. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballBallGameplayComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballBallGameplayComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Pass")
    float ShortPassSpeed = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Pass")
    float LongPassSpeed = 1450.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Pass")
    float ThroughPassSpeed = 1250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Pass")
    float CrossSpeed = 1300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Shot")
    float NormalShotSpeed = 1900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball|Shot")
    float PowerShotSpeed = 2500.0f;

    UFUNCTION(BlueprintCallable, Category="Ball|Gameplay")
    bool KickBall(AFootballBall* Ball, const FVector& Direction, EFootballKickType KickType, float Charge = 1.0f);

private:
    float GetKickSpeed(EFootballKickType KickType, float Charge) const;
};
