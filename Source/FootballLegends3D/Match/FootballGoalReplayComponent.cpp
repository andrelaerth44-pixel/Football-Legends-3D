#include "Match/FootballGoalReplayComponent.h"
#include "Ball/FootballBall.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"

UFootballGoalReplayComponent::UFootballGoalReplayComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFootballGoalReplayComponent::BeginPlay()
{
    Super::BeginPlay();
    Frames.Reserve(FMath::CeilToInt(BufferDuration / FMath::Max(SampleInterval, 0.01f)) + 2);

    // The recorder must already be running before the goal happens so the
    // rolling buffer contains the approach and the actual shot.
    if (UWorld* World = GetWorld())
    {
        for (TActorIterator<AFootballBall> It(World); It; ++It)
        {
            SetTrackedBall(*It);
            break;
        }
    }
}

void UFootballGoalReplayComponent::SetTrackedBall(AFootballBall* Ball)
{
    if (bReplaying)
    {
        return;
    }

    if (TrackedBall == Ball)
    {
        return;
    }

    TrackedBall = Ball;
    Frames.Reset();
    SampleAccumulator = 0.0f;
}

void UFootballGoalReplayComponent::CaptureFrame()
{
    if (!IsValid(TrackedBall))
    {
        return;
    }

    FFootballReplayFrame Frame;
    Frame.Time = Frames.Num() * SampleInterval;
    Frame.BallLocation = TrackedBall->GetActorLocation();
    Frame.BallRotation = TrackedBall->GetActorRotation();
    Frame.BallVelocity = TrackedBall->BallMesh ? TrackedBall->BallMesh->GetPhysicsLinearVelocity() : FVector::ZeroVector;

    Frames.Add(Frame);

    const int32 MaxFrames = FMath::Max(2, FMath::CeilToInt(BufferDuration / FMath::Max(SampleInterval, 0.01f)) + 1);
    while (Frames.Num() > MaxFrames)
    {
        Frames.RemoveAt(0);
    }

    for (int32 Index = 0; Index < Frames.Num(); ++Index)
    {
        Frames[Index].Time = Index * SampleInterval;
    }
}

void UFootballGoalReplayComponent::StartReplay()
{
    if (bReplaying || Frames.Num() < 2 || !IsValid(TrackedBall))
    {
        return;
    }

    bReplaying = true;
    ReplayTime = 0.0f;
    ReplayDuration = Frames.Last().Time;

    if (TrackedBall->BallMesh)
    {
        TrackedBall->BallMesh->SetSimulatePhysics(false);
        TrackedBall->BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        TrackedBall->BallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

    ApplyFrame(Frames[0]);
    OnReplayStarted.Broadcast();
}

void UFootballGoalReplayComponent::StopReplay()
{
    if (!bReplaying)
    {
        return;
    }

    bReplaying = false;
    ReplayTime = 0.0f;

    if (TrackedBall && TrackedBall->BallMesh)
    {
        const FVector LastVelocity = Frames.Num() > 0 ? Frames.Last().BallVelocity : FVector::ZeroVector;
        TrackedBall->BallMesh->SetSimulatePhysics(true);
        TrackedBall->BallMesh->SetPhysicsLinearVelocity(LastVelocity);
        TrackedBall->BallMesh->WakeAllRigidBodies();
    }

    OnReplayFinished.Broadcast();
}

bool UFootballGoalReplayComponent::IsReplaying() const
{
    return bReplaying;
}

float UFootballGoalReplayComponent::GetReplayProgress() const
{
    return ReplayDuration > KINDA_SMALL_NUMBER
        ? FMath::Clamp(ReplayTime / ReplayDuration, 0.0f, 1.0f)
        : 0.0f;
}

void UFootballGoalReplayComponent::ApplyFrame(const FFootballReplayFrame& Frame)
{
    if (!IsValid(TrackedBall))
    {
        return;
    }

    TrackedBall->SetActorLocationAndRotation(Frame.BallLocation, Frame.BallRotation, false, nullptr, ETeleportType::TeleportPhysics);
}

void UFootballGoalReplayComponent::UpdateReplay(float DeltaTime)
{
    if (Frames.Num() < 2)
    {
        StopReplay();
        return;
    }

    ReplayTime += FMath::Max(0.0f, DeltaTime) * ReplaySpeed;
    if (ReplayTime >= ReplayDuration)
    {
        ApplyFrame(Frames.Last());
        StopReplay();
        return;
    }

    int32 NextIndex = 1;
    while (NextIndex < Frames.Num() && Frames[NextIndex].Time < ReplayTime)
    {
        ++NextIndex;
    }

    const int32 PreviousIndex = FMath::Clamp(NextIndex - 1, 0, Frames.Num() - 1);
    NextIndex = FMath::Clamp(NextIndex, 0, Frames.Num() - 1);

    const FFootballReplayFrame& A = Frames[PreviousIndex];
    const FFootballReplayFrame& B = Frames[NextIndex];
    const float Denominator = FMath::Max(KINDA_SMALL_NUMBER, B.Time - A.Time);
    const float Alpha = FMath::Clamp((ReplayTime - A.Time) / Denominator, 0.0f, 1.0f);

    FFootballReplayFrame Interpolated;
    Interpolated.BallLocation = FMath::Lerp(A.BallLocation, B.BallLocation, Alpha);
    Interpolated.BallRotation = FMath::Lerp(A.BallRotation, B.BallRotation, Alpha);
    Interpolated.BallVelocity = FMath::Lerp(A.BallVelocity, B.BallVelocity, Alpha);
    ApplyFrame(Interpolated);
}

void UFootballGoalReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bReplaying)
    {
        UpdateReplay(DeltaTime);
        return;
    }

    if (!IsValid(TrackedBall))
    {
        return;
    }

    SampleAccumulator += FMath::Max(0.0f, DeltaTime);
    const float SafeInterval = FMath::Max(SampleInterval, 0.01f);

    while (SampleAccumulator >= SafeInterval)
    {
        SampleAccumulator -= SafeInterval;
        CaptureFrame();
    }
}
