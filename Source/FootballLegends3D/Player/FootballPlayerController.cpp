#include "Player/FootballPlayerController.h"
#include "Camera/FootballCameraActor.h"
#include "Characters/FootballPlayer.h"
#include "Characters/FootballPlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

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
}

void AFootballPlayerController::MoveForward(float Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
        ControlledPawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AFootballPlayerController::MoveRight(float Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
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
