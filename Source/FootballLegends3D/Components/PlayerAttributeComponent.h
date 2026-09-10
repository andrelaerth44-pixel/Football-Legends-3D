#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttributeComponent.generated.h"

/** Data-driven football attributes. Values are intentionally independent from the player mesh. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UPlayerAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerAttributeComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Speed = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Acceleration = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Dribbling = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Passing = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Shooting = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Defending = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Strength = 70;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin="1", ClampMax="99"))
    int32 Vision = 70;
};
