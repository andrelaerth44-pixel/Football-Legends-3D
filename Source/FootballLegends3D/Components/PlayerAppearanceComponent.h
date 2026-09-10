#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAppearanceComponent.generated.h"

/** Keeps player appearance separate from gameplay logic and attributes. */
UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UPlayerAppearanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerAppearanceComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FName PlayerId = TEXT("player_default");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FName TeamId = TEXT("team_default");
};
