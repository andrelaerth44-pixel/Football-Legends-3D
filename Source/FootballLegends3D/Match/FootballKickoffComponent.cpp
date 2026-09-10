#include "Match/FootballKickoffComponent.h"
#include "Ball/FootballBall.h"
#include "Characters/FootballPlayer.h"
#include "Teams/FootballTeamComponent.h"
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

void UFootballKickoffComponent::SetKickoffSide(EFootballTeamSide NewSide)
{
    if (KickoffSide == NewSide && bKickoffReady)
    {
        return;
    }

    KickoffSide = NewSide;
    bKickoffReady = false;
    KickoffPlayer = nullptr;
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
    if (!GetWorld() || !IsValid(MatchBall))
    {
        return nullptr;
    }

    AFootballPlayer* BestPlayer = nullptr;
    float BestDistanceSq = TNumericLimits<float>::Max();

    for (TActorIterator<AFootballPlayer> It(GetWorld()); It; ++It)
    {
        AFootballPlayer* Player = *It;
        if (!IsValid(Player) || !Player->Team || !Player->Team->IsOnSide(Side))
        {
            continue;
        }

        const float DistanceSq = FVector::DistSquared(Player->GetActorLocation(), MatchBall->GetActorLocation());
        if (DistanceSq < BestDistanceSq)
        {
            BestDistanceSq = DistanceSq;
            BestPlayer = Player;
        }
    }

    return BestPlayer;
}

FVector UFootballKickoffComponent::GetFormationWorldLocation(
    const FVector2D& NormalizedPosition,
    EFootballTeamSide Side,
    const FVector& Center) const
{
    const float Direction = Side == EFootballTeamSide::Home ? 1.0f : -1.0f;
    return Center + FVector(
        NormalizedPosition.X * PitchHalfLength * Direction,
        NormalizedPosition.Y * PitchHalfWidth,
        0.0f);
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

    const FVector CenterLocation = MatchBall->GetActorLocation() - FVector::UpVector * KickoffBallHeight;
    MatchBall->SetActorLocation(CenterLocation + FVector::UpVector * KickoffBallHeight);
    MatchBall->SetActorRotation(FRotator::ZeroRotator);
}

void UFootballKickoffComponent::SetPlayersForKickoff()
{
    if (!GetWorld() || !IsValid(MatchBall))
    {
        return;
    }

    const FVector Center = MatchBall->GetActorLocation() - FVector::UpVector * KickoffBallHeight;
    int32 HomeFallbackIndex = 0;
    int32 AwayFallbackIndex = 0;

    for (TActorIterator<AFootballPlayer> It(GetWorld()); It; ++It)
    {
        AFootballPlayer* Player = *It;
        if (!IsValid(Player) || !Player->Team)
        {
            continue;
        }

        if (Player->Team->Team.Formation.Num() == 0)
        {
            Player->Team->BuildDefaultFormation();
        }

        const int32 FormationCount = Player->Team->Team.Formation.Num();
        if (FormationCount == 0)
        {
            continue;
        }

        const FFootballFormationSlot* Slot = Player->Team->FindSlotByShirtNumber(Player->Team->ShirtNumber);
        if (!Slot)
        {
            int32& FallbackIndex = Player->Team->Side == EFootballTeamSide::Home ? HomeFallbackIndex : AwayFallbackIndex;
            Slot = &Player->Team->Team.Formation[FallbackIndex % FormationCount];
            ++FallbackIndex;
        }

        const FVector Target = GetFormationWorldLocation(Slot->NormalizedPosition, Player->Team->Side, Center);
        Player->SetActorLocation(Target);
        Player->SetActorRotation(FRotator(0.0f, Player->Team->Side == EFootballTeamSide::Home ? 0.0f : 180.0f, 0.0f));

        if (Player->GetCharacterMovement())
        {
            Player->GetCharacterMovement()->StopMovementImmediately();
        }
    }

    KickoffPlayer = FindKickoffPlayer(KickoffSide);
}

void UFootballKickoffComponent::PrepareKickoff()
{
    if (bKickoffReady)
    {
        return;
    }

    MatchBall = FindMatchBall();
    if (!IsValid(MatchBall))
    {
        return;
    }

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
    else if (UStaticMeshComponent* Mesh = MatchBall->BallMesh)
    {
        Mesh->SetSimulatePhysics(true);
        Mesh->WakeAllRigidBodies();
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
