#include "Ball/FootballBallPossessionComponent.h"
#include "Ball/FootballBall.h"
#include "Components/FootballBallInteractionComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

UFootballBallPossessionComponent::UFootballBallPossessionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool UFootballBallPossessionComponent::AcquireBall(AFootballBall* Ball)
{
    if (!IsValid(Ball) || !GetOwner())
    {
        return false;
    }

    if (ControlledBall == Ball)
    {
        return true;
    }

    if (IsValid(ControlledBall))
    {
        return false;
    }

    if (FVector::Dist(GetOwner()->GetActorLocation(), Ball->GetActorLocation()) > MaxControlDistance)
    {
        return false;
    }

    ControlledBall = Ball;
    Ball->StopBall();

    if (UPrimitiveComponent* Primitive = Ball->BallMesh)
    {
        Primitive->SetSimulatePhysics(false);
        Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
        Primitive->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
    }

    if (UFootballBallInteractionComponent* Interaction = GetOwner()->FindComponentByClass<UFootballBallInteractionComponent>())
    {
        Interaction->SetControlledBall(Ball);
    }

    return true;
}

void UFootballBallPossessionComponent::ReleaseBall()
{
    AFootballBall* Ball = ControlledBall.Get();
    ControlledBall = nullptr;

    if (Ball)
    {
        if (UPrimitiveComponent* Primitive = Ball->BallMesh)
        {
            Primitive->SetSimulatePhysics(true);
        }
    }

    if (GetOwner())
    {
        if (UFootballBallInteractionComponent* Interaction = GetOwner()->FindComponentByClass<UFootballBallInteractionComponent>())
        {
            Interaction->ReleaseControlledBall();
        }
    }
}

bool UFootballBallPossessionComponent::HasBall() const
{
    return IsValid(ControlledBall);
}

AFootballBall* UFootballBallPossessionComponent::GetControlledBall() const
{
    return ControlledBall.Get();
}

void UFootballBallPossessionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!IsValid(ControlledBall) || !GetOwner())
    {
        ControlledBall = nullptr;
        return;
    }

    const FVector OwnerLocation = GetOwner()->GetActorLocation();
    const FVector Forward = GetOwner()->GetActorForwardVector().GetSafeNormal2D();
    const FVector DesiredLocation = OwnerLocation + Forward * TouchDistance + FVector(0.0f, 0.0f, 20.0f);
    const FVector NewLocation = FMath::VInterpTo(ControlledBall->GetActorLocation(), DesiredLocation, DeltaTime, ControlSpeed);

    ControlledBall->SetActorLocation(NewLocation, false);
}
