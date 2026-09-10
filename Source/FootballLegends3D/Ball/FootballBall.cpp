#include "Ball/FootballBall.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"

AFootballBall::AFootballBall()
{
    PrimaryActorTick.bCanEverTick = false;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    SetRootComponent(BallMesh);

    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetEnableGravity(true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    BallMesh->SetLinearDamping(RollingFriction);
    BallMesh->SetAngularDamping(0.08f);
    BallMesh->SetMassOverrideInKg(NAME_None, BallMass, true);
    BallMesh->BodyInstance.Restitution = Restitution;
    BallMesh->BodyInstance.bOverrideMass = true;
}

void AFootballBall::Kick(const FVector& Direction, float Speed, float Spin)
{
    if (!BallMesh || Speed <= 0.0f)
    {
        return;
    }

    const FVector SafeDirection = Direction.GetSafeNormal();
    if (SafeDirection.IsNearlyZero())
    {
        return;
    }

    BallMesh->SetPhysicsLinearVelocity(SafeDirection * Speed, false);

    if (!FMath::IsNearlyZero(Spin))
    {
        BallMesh->AddAngularImpulseInRadians(FVector::UpVector * Spin, NAME_None, true);
    }
}

void AFootballBall::StopBall()
{
    if (BallMesh)
    {
        BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector, false);
        BallMesh->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector, false);
    }
}

bool AFootballBall::IsMoving() const
{
    return BallMesh && BallMesh->GetPhysicsLinearVelocity().SizeSquared() > FMath::Square(5.0f);
}
