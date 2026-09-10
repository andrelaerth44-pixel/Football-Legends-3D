#include "Ball/FootballBallTargetingComponent.h"
#include "Characters/FootballPlayer.h"
#include "Kismet/GameplayStatics.h"

UFootballBallTargetingComponent::UFootballBallTargetingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float UFootballBallTargetingComponent::ScoreTarget(const AFootballPlayer* Candidate) const
{
    const AActor* Owner = GetOwner();
    if (!Owner || !Candidate || Candidate == Owner)
    {
        return -FLT_MAX;
    }

    const FVector Offset = Candidate->GetActorLocation() - Owner->GetActorLocation();
    const float Distance = Offset.Size2D();
    if (Distance <= KINDA_SMALL_NUMBER || Distance > SearchRadius)
    {
        return -FLT_MAX;
    }

    const FVector Direction = Offset.GetSafeNormal2D();
    const FVector OwnerForward = Owner->GetActorForwardVector().GetSafeNormal2D();
    const float ForwardScore = FVector::DotProduct(OwnerForward, Direction);
    const float DistanceScore = 1.0f - FMath::Clamp(Distance / SearchRadius, 0.0f, 1.0f);

    return ForwardScore * 0.65f + DistanceScore * 0.35f;
}

AFootballPlayer* UFootballBallTargetingComponent::FindBestPassTarget() const
{
    if (!GetWorld() || !GetOwner())
    {
        return nullptr;
    }

    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFootballPlayer::StaticClass(), Players);

    AFootballPlayer* BestTarget = nullptr;
    float BestScore = -FLT_MAX;
    for (AActor* Actor : Players)
    {
        AFootballPlayer* Candidate = Cast<AFootballPlayer>(Actor);
        const float Score = ScoreTarget(Candidate);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestTarget = Candidate;
        }
    }

    return BestTarget;
}
