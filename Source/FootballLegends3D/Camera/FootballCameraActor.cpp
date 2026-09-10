#include "Camera/FootballCameraActor.h"
#include "Camera/CameraComponent.h"

AFootballCameraActor::AFootballCameraActor()
{
    PrimaryActorTick.bCanEverTick = false;

    if (UCameraComponent* Camera = GetCameraComponent())
    {
        Camera->FieldOfView = 45.0f;
        Camera->bConstrainAspectRatio = false;
    }
}
