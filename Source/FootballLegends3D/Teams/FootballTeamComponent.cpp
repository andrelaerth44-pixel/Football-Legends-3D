#include "Teams/FootballTeamComponent.h"

UFootballTeamComponent::UFootballTeamComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFootballTeamComponent::BuildDefaultFormation()
{
    Team.Formation.Reset();

    const EFootballPlayerRole Roles[11] = {
        EFootballPlayerRole::Goalkeeper,
        EFootballPlayerRole::Defender, EFootballPlayerRole::Defender, EFootballPlayerRole::Defender, EFootballPlayerRole::Defender,
        EFootballPlayerRole::Midfielder, EFootballPlayerRole::Midfielder, EFootballPlayerRole::Midfielder,
        EFootballPlayerRole::Forward, EFootballPlayerRole::Forward, EFootballPlayerRole::Forward
    };

    const FVector2D Positions[11] = {
        {-0.90f, 0.00f},
        {-0.60f, -0.65f}, {-0.62f, -0.22f}, {-0.62f, 0.22f}, {-0.60f, 0.65f},
        {-0.15f, -0.55f}, {-0.08f, 0.00f}, {-0.15f, 0.55f},
        {0.55f, -0.55f}, {0.62f, 0.00f}, {0.55f, 0.55f}
    };

    for (int32 Index = 0; Index < 11; ++Index)
    {
        FFootballFormationSlot Slot;
        Slot.Role = Roles[Index];
        Slot.ShirtNumber = Index + 1;
        Slot.NormalizedPosition = Positions[Index];
        Team.Formation.Add(Slot);
    }
}

bool UFootballTeamComponent::IsOnSide(EFootballTeamSide QuerySide) const
{
    return Side == QuerySide;
}

const FFootballFormationSlot* UFootballTeamComponent::FindSlotByShirtNumber(int32 ShirtNumber) const
{
    for (const FFootballFormationSlot& Slot : Team.Formation)
    {
        if (Slot.ShirtNumber == ShirtNumber)
        {
            return &Slot;
        }
    }

    return nullptr;
}
