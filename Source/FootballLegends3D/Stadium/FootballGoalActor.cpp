#include "Stadium/FootballGoalActor.h"
#include "Stadium/FootballGoalNetDeformationComponent.h"
#include "Ball/FootballBall.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"

AFootballGoalActor::AFootballGoalActor()
{
    PrimaryActorTick.bCanEverTick = false;

    GoalVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalVolume"));
    SetRootComponent(GoalVolume);
    GoalVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GoalVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
    GoalVolume->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    GoalVolume->SetGenerateOverlapEvents(true);
    GoalVolume->SetBoxExtent(FVector(180.0f, 360.0f, 120.0f));

    NetVisualRoot = CreateDefaultSubobject<USceneComponent>(TEXT("NetVisualRoot"));
    NetVisualRoot->SetupAttachment(GoalVolume);

    NetReaction = CreateDefaultSubobject<UFootballGoalNetDeformationComponent>(TEXT("NetReaction"));
}

void AFootballGoalActor::BeginPlay()
{
    Super::BeginPlay();

    if (NetReaction)
    {
        NetReaction->SetTargetComponent(NetVisualRoot);
    }

    GoalVolume->OnComponentBeginOverlap.AddDynamic(this, &AFootballGoalActor::OnGoalVolumeBeginOverlap);
}

void AFootballGoalActor::OnGoalVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AFootballBall* Ball = Cast<AFootballBall>(OtherActor);
    if (!Ball || !GetWorld())
    {
        return;
    }

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastGoalTime < GoalCooldown)
    {
        return;
    }

    const FVector Velocity = Ball->BallMesh
        ? Ball->BallMesh->GetPhysicsLinearVelocity()
        : FVector::ZeroVector;
    const float ImpactSpeed = Velocity.Size();

    if (ImpactSpeed < 100.0f)
    {
        return;
    }

    // The ball must cross the goal volume in the goal's forward direction.
    const FVector GoalForward = GetActorForwardVector().GetSafeNormal();
    const float ForwardSpeed = FVector::DotProduct(Velocity, GoalForward);
    if (ForwardSpeed <= 0.0f)
    {
        return;
    }

    LastGoalTime = CurrentTime;

    if (NetReaction)
    {
        NetReaction->ReactToGoal(Velocity, ImpactSpeed);
    }

    OnGoalScored.Broadcast(Ball, ImpactSpeed);
}

void AFootballGoalActor::ResetGoal()
{
    LastGoalTime = -1000.0f;
}
