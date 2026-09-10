#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Match/FootballMatchTypes.h"
#include "FootballKickoffComponent.generated.h"

class AFootballBall;
class AFootballPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFootballKickoffSignature, EFootballTeamSide, KickoffSide);

UCLASS(ClassGroup=(Football), meta=(BlueprintSpawnableComponent))
class FOOTBALLLEGENDS3D_API UFootballKickoffComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootballKickoffComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kickoff")
    EFootballTeamSide DefaultKickoffSide = EFootballTeamSide::Home;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kickoff")
    float CenterTolerance = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kickoff")
    float KickoffBallHeight = 25.0f;

    UPROPERTY(BlueprintAssignable, Category="Kickoff")
    FFootballKickoffSignature OnKickoffPrepared;

    UPROPERTY(BlueprintAssignable, Category="Kickoff")
    FFootballKickoffSignature OnKickoffStarted;

    UFUNCTION(BlueprintCallable, Category="Kickoff")
    void PrepareKickoff();

    UFUNCTION(BlueprintCallable, Category="Kickoff")
    void StartKickoff();

    UFUNCTION(BlueprintPure, Category="Kickoff")
    bool IsKickoffReady() const;

    UFUNCTION(BlueprintPure, Category="Kickoff")
    EFootballTeamSide GetKickoffSide() const;

protected:
    virtual void BeginPlay() override;

private:
    AFootballBall* FindMatchBall() const;
    AFootballPlayer* FindKickoffPlayer(EFootballTeamSide Side) const;
    void SetPlayersForKickoff();
    void ResetBallForKickoff();

    UPROPERTY()
    TObjectPtr<AFootballBall> MatchBall;

    UPROPERTY()
    TObjectPtr<AFootballPlayer> KickoffPlayer;

    EFootballTeamSide KickoffSide = EFootballTeamSide::Home;
    bool bKickoffReady = false;
};
