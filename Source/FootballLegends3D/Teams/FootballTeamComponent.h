#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Match/FootballMatchTypes.h"
#include "Teams/FootballTeamTypes.h"
#include "FootballTeamComponent.generated.h"

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballTeamComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballTeamComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
    FFootballTeamDefinition Team;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
    EFootballTeamSide Side = EFootballTeamSide::Home;

    UFUNCTION(BlueprintCallable, Category="Team")
    void BuildDefaultFormation();

    UFUNCTION(BlueprintPure, Category="Team")
    bool IsOnSide(EFootballTeamSide QuerySide) const;

    UFUNCTION(BlueprintPure, Category="Team")
    const FFootballFormationSlot* FindSlotByShirtNumber(int32 ShirtNumber) const;
};
