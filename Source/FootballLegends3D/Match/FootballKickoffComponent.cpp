#include "Match/FootballKickoffComponent.h"
#include "Ball/FootballBall.h"
#include "Characters/FootballPlayer.h"
#include "Components/FootballBallInteractionComponent.h"
#include "Ball/FootballBallPossessionComponent.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UFootballKickoffComponent::UFootballKickoffComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFootballKickoffComponent::BeginPlay()
{
    Super::BeginPlay();
    KickoffSide = DefaultKickoffSide;
    MatchBall = FindMatchBall();
}

AFootballBall* UFootballKickoffComponent::FindMatchBall() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AFootballBall> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

AFootballPlayer* UFootballKickoffComponent::FindKickoffPlayer(EFootballTeamSide Side) const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AFootballPlayer* BestPlayer = nullptr;
    float BestDistanceSq = TNumericLimits<float>::Max();

    if (!MatchBall)
    {
        return nullptr;
    }

    for (TActorIterator<AFootballPlayer> It(GetWorld()); It; ++It)
    {
        AFootballPlayer* Player = *It;
        if (!IsValid(Player))
        {
            continue;
        }

        // Team side is data-driven once TeamDefinition is introduced. Until
        // then, choose the nearest valid player to the center spot.
        const float DistanceSq = FVector::DistSquared(Player->GetActorLocation(), MatchBall->GetActorLocation());
        if (DistanceSq < BestDistanceSq)
        {
            BestDistanceSq = DistanceSq;
            BestPlayer = Player;
        }
    }

    return BestPlayer;
}

void UFootballKickoffComponent::ResetBallForKickoff()
{
    if (!IsValid(MatchBall))
    {
        MatchBall = FindMatchBall();
    }

    if (!IsValid(MatchBall))
    {
        return;
    }

    if (UStaticMeshComponent* Mesh = MatchBall->BallMesh)
    {
        Mesh->SetSimulatePhysics(false);
        Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

    const FVector CenterLocation = MatchBall->GetActorLocation();
    MatchBall->SetActorLocation(CenterLocation + FVector::UpVector * KickoffBallHeight);
    MatchBall->SetActorRotation(FRotator::ZeroRotator);
}

void UFootballKickoffComponent::SetPlayersForKickoff()
{
    KickoffPlayer = FindKickoffPlayer(KickoffSide);

    if (IsValid(KickoffPlayer) && KickoffPlayer->GetCharacterMovement())
    {
        KickoffPlayer->GetCharacterMovement()->StopMovementImmediately();
    }

    // Existing player locations are intentionally preserved here. Full team
    // formation placement will be driven by TeamDefinition/Formation data.
}

void UFootballKickoffComponent::PrepareKickoff()
{
    if (bKickoffReady)
    {
        return;
    }

    MatchBall = FindMatchBall();
    ResetBallForKickoff();
    SetPlayersForKickoff();

    bKickoffReady = IsValid(MatchBall);
    if (bKickoffReady)
    {
        OnKickoffPrepared.Broadcast(KickoffSide);
    }
}

void UFootballKickoffComponent::StartKickoff()
{
    if (!bKickoffReady)
    {
        PrepareKickoff();
    }

    if (!bKickoffReady || !IsValid(MatchBall))
    {
        return;
    }

    if (IsValid(KickoffPlayer) && KickoffPlayer->BallPossession)
    {
        KickoffPlayer->BallPossession->AcquireBall(MatchBall);
    }

    bKickoffReady = false;
    OnKickoffStarted.Broadcast(KickoffSide);
}

bool UFootballKickoffComponent::IsKickoffReady() const
{
    return bKickoffReady;
}

EFootballTeamSide UFootballKickoffComponent::GetKickoffSide() const
{
    return KickoffSide;
}
