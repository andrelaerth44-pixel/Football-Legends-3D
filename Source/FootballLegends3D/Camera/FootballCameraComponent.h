#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootballCameraComponent.generated.h"

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballCameraComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballCameraComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin="0.0"))
    float Distance = 950.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin="0.0"))
    float Height = 620.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin="0.0"))
    float FollowSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin="0.0"))
    float LookAhead = 250.0f;
};
