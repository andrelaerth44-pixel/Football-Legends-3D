#include "Ball/FootballBall.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"

AFootballBall::AFootballBall()
{
    PrimaryActorTick.bCanEverTick = true;

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
    RestScale = BallMesh->GetRelativeScale3D();
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

    TriggerDeformation(SafeDirection, Speed);
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

void AFootballBall::TriggerDeformation(const FVector& Direction, float Speed)
{
    if (!BallMesh || MaxDeformation <= 0.0f)
    {
        return;
    }

    DeformationTimeRemaining = DeformationDuration;

    const float Intensity = FMath::Clamp(Speed / 2500.0f, 0.0f, 1.0f);
    const float Squash = MaxDeformation * Intensity;
    const FVector LocalAxis = GetActorTransform().InverseTransformVectorNoScale(Direction).GetSafeNormal();
    const FVector AxisAbs(FMath::Abs(LocalAxis.X), FMath::Abs(LocalAxis.Y), FMath::Abs(LocalAxis.Z));

    ImpactScale = RestScale * FVector(
        1.0f - Squash * (1.0f - AxisAbs.X),
        1.0f - Squash * (1.0f - AxisAbs.Y),
        1.0f - Squash * (1.0f - AxisAbs.Z));

    BallMesh->SetRelativeScale3D(ImpactScale);
}

void AFootballBall::UpdateDeformation(float DeltaSeconds)
{
    if (!BallMesh || DeformationTimeRemaining <= 0.0f)
    {
        return;
    }

    DeformationTimeRemaining = FMath::Max(0.0f, DeformationTimeRemaining - DeltaSeconds);
    const float Alpha = DeformationDuration > 0.0f
        ? 1.0f - (DeformationTimeRemaining / DeformationDuration)
        : 1.0f;

    const float SmoothAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
    BallMesh->SetRelativeScale3D(FMath::Lerp(ImpactScale, RestScale, SmoothAlpha));

    if (DeformationTimeRemaining <= KINDA_SMALL_NUMBER)
    {
        BallMesh->SetRelativeScale3D(RestScale);
    }
}

void AFootballBall::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateDeformation(DeltaSeconds);
}
