# Football Legends 3D

Professional 3D football project built with **Unreal Engine 5 + C++**, targeting mobile/tablet first and designed for expansion to other platforms.

## Core direction

- Responsive football gameplay inspired by modern football games
- Modular C++ gameplay architecture
- Data-driven players, teams, attributes and abilities
- Dedicated ball interaction and player movement systems
- Animation Blueprint / Blend Space / IK pipeline
- Mobile touch controls from the beginning
- AI architecture for teammates, opponents and goalkeepers

## Player system — Phase 1

The first gameplay foundation is built around a reusable `AFootballPlayer` character and modular components for attributes, appearance and ball interaction.

### Planned structure

```text
Source/FootballLegends3D/
├── Characters/
│   └── FootballPlayer.h/.cpp
├── Components/
│   ├── PlayerAttributeComponent.h/.cpp
│   ├── PlayerAppearanceComponent.h/.cpp
│   └── FootballBallInteractionComponent.h/.cpp
├── AI/
├── Animation/
├── Ball/
├── Teams/
└── UI/
```

> Official Supa Strikas characters, names, logos and other protected assets require the appropriate rights/licensing for commercial distribution. The codebase is structured so licensed content or original characters can be plugged into the same systems.

## Development rule

Build incrementally, keep systems modular, and avoid breaking existing gameplay when new features are introduced.
