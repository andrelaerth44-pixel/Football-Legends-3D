#include "Player/FootballPlayerController.h"
#include "Camera/FootballCameraActor.h"
#include "Characters/FootballPlayer.h"
#include "Characters/FootballPlayerMovementComponent.h"
#include "Ball/FootballBall.h"
#include "Ball/FootballBallGameplayComponent.h"
#include "Ball/FootballBallPossessionComponent.h"
#include "Components/FootballBallInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AFootballPlayerController::AFootballPlayerController()
{
    bShowMouseCursor = false;
}

void AFootballPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (APawn* ControlledPawn = GetPawn())
    {
        FootballCamera = GetWorld()->SpawnActor<AFootballCameraActor>(AFootballCameraActor::StaticClass());
        if (FootballCamera)
        {
            FootballCamera->SetFollowTarget(ControlledPawn);
            SetViewTarget(FootballCamera);
        }

        UpdateMovementSpeed();
    }
}

void AFootballPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis("MoveForward", this, &AFootballPlayerController::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AFootballPlayerController::MoveRight);
    InputComponent->BindAction("Sprint", IE_Pressed, this, &AFootballPlayerController::StartSprint);
    InputComponent->BindAction("Sprint", IE_Released, this, &AFootballPlayerController::StopSprint);
    InputComponent->BindAction("Pass", IE_Pressed, this, &AFootballPlayerController::StartPass);
    InputComponent->BindAction("Pass", IE_Released, this, &AFootballPlayerController::ReleasePass);
    InputComponent->BindAction("Shot", IE_Pressed, this, &AFootballPlayerController::StartShot);
    InputComponent->BindAction("Shot", IE_Released, this, &AFootballPlayerController::ReleaseShot);
}

void AFootballPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    const float ChargeDelta = DeltaTime * ChargeRate;
    if (bPassHeld)
    {
        PassCharge = FMath::Clamp(PassCharge + ChargeDelta, 0.0f, 1.0f);
    }
    if (bShotHeld)
    {
        ShotCharge = FMath::Clamp(ShotCharge + ChargeDelta, 0.0f, 1.0f);
    }
}

void AFootballPlayerController::MoveForward(float Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
        ControlledPawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AFootballPlayerController::MoveRight(float Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
        ControlledPawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AFootballPlayerController::StartSprint()
{
    bSprintHeld = true;
    UpdateMovementSpeed();
}

void AFootballPlayerController::StopSprint()
{
    bSprintHeld = false;
    UpdateMovementSpeed();
}

void AFootballPlayerController::StartPass()
{
    bPassHeld = true;
    PassCharge = 0.0f;
}

void AFootballPlayerController::ReleasePass()
{
    if (!bPassHeld)
    {
        return;
    }

    bPassHeld = false;
    Kick(EFootballKickType::ShortPass, PassCharge);
    PassCharge = 0.0f;
}

void AFootballPlayerController::StartShot()
{
    bShotHeld = true;
    ShotCharge = 0.0f;
}

void AFootballPlayerController::ReleaseShot()
{
    if (!bShotHeld)
    {
        return;
    }

    bShotHeld = false;
    Kick(EFootballKickType::NormalShot, ShotCharge);
    ShotCharge = 0.0f;
}

void AFootballPlayerController::Kick(EFootballKickType KickType, float Charge)
{
    AFootballPlayer* Player = Cast<AFootballPlayer>(GetPawn());
    if (!Player || !Player->BallGameplay || !Player->BallPossession || !Player->BallInteraction)
    {
        return;
    }

    AFootballBall* Ball = Player->BallPossession->HasBall()
        ? Player->BallInteraction->GetControlledBall()
        : FindControllableBall();

    if (!Ball)
    {
        return;
    }

    if (!Player->BallPossession->HasBall() && !Player->BallPossession->AcquireBall(Ball))
    {
        return;
    }

    if (Player->BallGameplay->KickBall(Ball, GetKickDirection(), KickType, Charge))
    {
        Player->BallPossession->ReleaseBall();
        Player->BallInteraction->ReleaseControlledBall();
    }
}

AFootballBall* AFootballPlayerController::FindControllableBall() const
{
    AFootballPlayer* Player = Cast<AFootballPlayer>(GetPawn());
    if (!Player || !Player->BallInteraction)
    {
        return nullptr;
    }

    TArray<AActor*> Balls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFootballBall::StaticClass(), Balls);

    AFootballBall* BestBall = nullptr;
    float BestDistanceSq = TNumericLimits<float>::Max();
    for (AActor* Actor : Balls)
    {
        AFootballBall* Ball = Cast<AFootballBall>(Actor);
        if (!Ball || !Player->BallInteraction->CanControlBall(Ball))
        {
            continue;
        }

        const float DistanceSq = FVector::DistSquared(Player->GetActorLocation(), Ball->GetActorLocation());
        if (DistanceSq < BestDistanceSq)
        {
            BestDistanceSq = DistanceSq;
            BestBall = Ball;
        }
    }

    return BestBall;
}

FVector AFootballPlayerController::GetKickDirection() const
{
    const FVector2D MoveInput(GetInputAxisValue("MoveRight"), GetInputAxisValue("MoveForward"));
    const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

    if (MoveInput.SizeSquared() > KINDA_SMALL_NUMBER)
    {
        return FRotationMatrix(YawRotation).TransformVector(FVector(MoveInput.Y, MoveInput.X, 0.0f)).GetSafeNormal();
    }

    if (APawn* ControlledPawn = GetPawn())
    {
        return ControlledPawn->GetActorForwardVector();
    }

    return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
}

void AFootballPlayerController::UpdateMovementSpeed()
{
    AFootballPlayer* Player = Cast<AFootballPlayer>(GetPawn());
    if (!Player)
    {
        return;
    }

    UCharacterMovementComponent* CharacterMovement = Player->GetCharacterMovement();
    UFootballPlayerMovementComponent* FootballMovement = Player->FootballMovement;
    if (CharacterMovement && FootballMovement)
    {
        CharacterMovement->MaxWalkSpeed = FootballMovement->GetTargetSpeed(bSprintHeld);
        CharacterMovement->MaxAcceleration = FootballMovement->Acceleration;
    }
}
