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
        if (!IsValid(Player))
        {
            continue;
        }

        const UFootballTeamComponent* TeamComponent = Player->FindComponentByClass<UFootballTeamComponent>();
        if (!TeamComponent || !TeamComponent->IsOnSide(Side))
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

    // The existing ball location defines the pitch center. This keeps the
    // system independent of a particular stadium actor or map origin.
    const FVector CenterLocation = MatchBall->GetActorLocation();
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

    for (TActorIterator<AFootballPlayer> It(GetWorld()); It; ++It)
    {
        AFootballPlayer* Player = *It;
        if (!IsValid(Player))
        {
            continue;
        }

        UFootballTeamComponent* TeamComponent = Player->FindComponentByClass<UFootballTeamComponent>();
        if (!TeamComponent)
        {
            continue;
        }

        if (TeamComponent->Team.Formation.Num() == 0)
        {
            TeamComponent->BuildDefaultFormation();
        }

        const int32 ShirtNumber = TeamComponent->FindSlotByShirtNumber(
            Player->Appearance ? FCString::Atoi(*Player->Appearance->PlayerId.ToString()) : 0);

        const FFootballFormationSlot* Slot = nullptr;
        if (ShirtNumber > 0)
        {
            Slot = TeamComponent->FindSlotByShirtNumber(ShirtNumber);
        }

        // Until player identity data exposes an explicit shirt number, use
        // the actor's existing shirt-number-compatible slot when available.
        if (!Slot)
        {
            const int32 Index = FMath::Clamp(Player->GetUniqueID() % TeamComponent->Team.Formation.Num(), 0, TeamComponent->Team.Formation.Num() - 1);
            Slot = &TeamComponent->Team.Formation[Index];
        }

        if (Slot)
        {
            const FVector Target = GetFormationWorldLocation(Slot->NormalizedPosition, TeamComponent->Side, Center);
            Player->SetActorLocation(Target);
            Player->SetActorRotation(FRotator(0.0f, TeamComponent->Side == EFootballTeamSide::Home ? 0.0f : 180.0f, 0.0f));
        }

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

    // Preserve the map-defined ball position as the center spot, then arrange
    // both teams around that spot according to their formation data.
    ResetBallForKickoff();
    SetPlayersForKickoff();

    bKickoffReady = IsValid(KickoffPlayer) || IsValid(MatchBall);
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
