// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameMode/DigumGamePrimaryGameMode.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
#include "Asset/DigumAssetManager.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "GameState/DigumGamePrimaryGameState.h"
#include "Interface/IDigumWorldPawnInterface.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMapHandler.h"
#include "Settings/DigumWorldSettings.h"

ADigumGamePrimaryGameMode::ADigumGamePrimaryGameMode()
{
	GameStateClass = ADigumGamePrimaryGameState::StaticClass();
}

void ADigumGamePrimaryGameMode::InitGameState()
{
	Super::InitGameState();

	SetWorldSeed(1234);

	if(ADigumGamePrimaryGameState* DigumGameState = GetGameState<ADigumGamePrimaryGameState>())
	{
		// TODO
	}
	
}

void ADigumGamePrimaryGameMode::StartPlay()
{
	Super::StartPlay();

	WorldMapHandler = NewObject<UDigumWorldMapHandler>(this);
	UDigumWorldProceduralAsset* Asset = UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);
	
	if(WorldMapHandler && Asset)
	{
		WorldMapHandler->SetProceduralAsset(Asset);
		WorldMapHandler->GenerateMap(TEXT("Hello World"), 10, 10, 2, 2, 2);

		FDigumWorldMap Map = WorldMapHandler->GetMap();
		const int32 LocalSectionWidth = Map.GetLocalSectionWidth();
		const int32 LocalSectionHeight = Map.GetLocalSectionHeight();
		const FVector GridSize = GetGridSize();
		const float TotalSectionWidth = LocalSectionWidth * GridSize.X;
		const float TotalSectionHeight = LocalSectionHeight * GridSize.Z;
		
		// Spawn Dynamic Procedural Actor
		if(Map.IsInitialized())
		{
			ProceduralActor = GetWorld()->SpawnActorDeferred<ADigumWorldDynamicProceduralActor>(ADigumWorldDynamicProceduralActor::StaticClass(), FTransform::Identity);
			ProceduralActor->Initialize(LocalSectionWidth, LocalSectionHeight, GridSize);
			ProceduralActor->FinishSpawning(FTransform::Identity);

			const FVector StartLocation = FVector(0, 0, 0) - GetWorldOffset();
			FDigumWorldProceduralSectionCoordinate Coordinate;
			UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(StartLocation, FVector2D(TotalSectionWidth, TotalSectionHeight), Coordinate);

			FDigumWorldProceduralSection Section;
			if(WorldMapHandler->GetSection(Coordinate.X, Coordinate.Y, Section))
			{
				ProceduralActor->CreateSection(Section, FVector::ZeroVector, Asset);
			}

			ProceduralActor->SetActorLocation(GetWorldOffset());

			UE_LOG(LogTemp, Warning, TEXT("Dynamic Procedural Actor spawned"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Map is not initialized"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldMapHandler or Asset is null"));
	}
}

void ADigumGamePrimaryGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// this function doesnt spawn the character yet
}

void ADigumGamePrimaryGameMode::RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent)
{
	if(InComponent)
	{
		InComponent->GetOnCoordinateChangedDelegate().AddUObject(this, &ADigumGamePrimaryGameMode::HandleCharacterCoordinateChanged);
	}
}

int32 ADigumGamePrimaryGameMode::GetWorldSeed() const
{
	return WorldSeed;
}

void ADigumGamePrimaryGameMode::SetWorldSeed(const int32& InValue)
{
	if(InValue >= 0)
		WorldSeed = InValue;
}

FVector ADigumGamePrimaryGameMode::GetGridSize() const
{
	const UDigumWorldSettings* Settings = UDigumWorldSettings::Get();
	if(Settings)
	{
		return Settings->GetGridSize();
	}
	return FVector::ZeroVector;
}

FVector2D ADigumGamePrimaryGameMode::GetSectionSize() const
{
	FVector GridSize = GetGridSize();
	if(WorldMapHandler && WorldMapHandler->GetMap().IsInitialized())
	{
		const int32 LocalSectionWidth = WorldMapHandler->GetMap().SectionWidth;
		const int32 LocalSectionHeight = WorldMapHandler->GetMap().SectionHeight;
		return FVector2D(LocalSectionWidth * GridSize.X, LocalSectionHeight * GridSize.Z);
	}
	return FVector2D::ZeroVector;
}


FVector ADigumGamePrimaryGameMode::GetWorldOffset() const
{
	if(!WorldMapHandler && !WorldMapHandler->GetMap().IsInitialized()) return FVector::ZeroVector;
	const FVector GridSize = GetGridSize();
	const int32 SectionWidth = WorldMapHandler->GetMap().SectionWidth;
	const int32 SectionHeight = WorldMapHandler->GetMap().SectionHeight;
	const int32 SectionCount_HorizontalAxis = WorldMapHandler->GetMap().SectionCount_HorizontalAxis;
	const int32 SectionCount_VerticalAxis = WorldMapHandler->GetMap().SectionCount_VerticalAxis;
	
	const float TotalSectionWidth = SectionWidth * GridSize.X;
	const float TotalSectionHeight = SectionHeight * GridSize.Z;

	const float TotalWidth = SectionCount_HorizontalAxis * TotalSectionWidth;
	const float TotalHeight =SectionCount_VerticalAxis * TotalSectionHeight;

	return FVector(-TotalWidth / 2, 0, TotalHeight / 2);
}

void ADigumGamePrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void ADigumGamePrimaryGameMode::HandleCharacterCoordinateChanged(const AActor* InActor,
	const FDigumWorldProceduralSectionCoordinate& InPreviousCoordinate,
	const FDigumWorldProceduralSectionCoordinate& InCurrentCoordinate)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Coordinate Changed"));
	if(WorldMapHandler == nullptr) return;

	UDigumWorldProceduralAsset* Asset = WorldMapHandler->GetProceduralAsset();
	

	const int32 Max = 2 - 1;
	TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = ProceduralActor->GetSectionCoordinatesInRadius(InCurrentCoordinate, 2, 0,  Max, 0, Max);

	for(const auto& Coordinate : Coordinates)
	{
		FDigumWorldProceduralSection Section;
		UE_LOG(LogTemp, Warning, TEXT("Coordinate: %s"), *Coordinate.ToString());
		if(WorldMapHandler->GetSection(Coordinate.X, Coordinate.Y, Section))
		{
			ProceduralActor->CreateSection(Section, GetWorldOffset(), Asset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Section not found"));
		}
	}

}
