#include "Player/FootballPlayerController.h"
#include "Characters/FootballPlayer.h"

AFootballPlayerController::AFootballPlayerController()
{
    bShowMouseCursor = false;
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
}

void AFootballPlayerController::StopSprint()
{
    bSprintHeld = false;
}
