#include "Match/FootballGoalReplayDirectorComponent.h"
#include "Match/FootballGoalReplayComponent.h"
#include "Camera/FootballGoalReplayCameraActor.h"
#include "Ball/FootballBall.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UFootballGoalReplayDirectorComponent::UFootballGoalReplayDirectorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFootballGoalReplayDirectorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        Replay = Owner->FindComponentByClass<UFootballGoalReplayComponent>();
    }

    if (UWorld* World = GetWorld())
    {
        PlayerController = World->GetFirstPlayerController();
    }
}

void UFootballGoalReplayDirectorComponent::StartGoalReplay(AFootballBall* Ball)
{
    if (!bAutoPlayCamera || !IsValid(Ball) || !GetWorld())
    {
        return;
    }

    if (!PlayerController)
    {
        PlayerController = GetWorld()->GetFirstPlayerController();
    }

    if (!PlayerController)
    {
        return;
    }

    if (!ReplayCamera)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ReplayCamera = GetWorld()->SpawnActor<AFootballGoalReplayCameraActor>(AFootballGoalReplayCameraActor::StaticClass(), Ball->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    }

    if (!ReplayCamera)
    {
        return;
    }

    PreviousViewTarget = PlayerController->GetViewTarget();
    ReplayCamera->SetReplayBall(Ball);
    ReplayCamera->SetReplayActive(true);
    PlayerController->SetViewTargetWithBlend(ReplayCamera, CameraBlendTime);

    if (Replay && !Replay->IsReplaying())
    {
        Replay->StartReplay();
    }
}

void UFootballGoalReplayDirectorComponent::StopGoalReplay()
{
    if (Replay && Replay->IsReplaying())
    {
        Replay->StopReplay();
    }

    if (ReplayCamera)
    {
        ReplayCamera->SetReplayActive(false);
    }

    if (PlayerController)
    {
        AActor* Target = PreviousViewTarget.Get();
        if (IsValid(Target))
        {
            PlayerController->SetViewTargetWithBlend(Target, CameraBlendTime);
        }
    }
}

bool UFootballGoalReplayDirectorComponent::IsGoalReplayActive() const
{
    return ReplayCamera && ReplayCamera->IsReplayActive();
}
