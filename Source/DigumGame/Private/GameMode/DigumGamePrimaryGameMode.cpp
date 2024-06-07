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
	UDigumWorldProceduralAsset* Asset = UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);
	if(Asset)
	{
		const FVector GridSize = GetGridSize();
		ProceduralActor = GetWorld()->SpawnActorDeferred<ADigumWorldDynamicProceduralActor>(ADigumWorldDynamicProceduralActor::StaticClass(), FTransform::Identity);
		ProceduralActor->GenerateMap(TEXT("Hello World"), GridSize,10, 10, 4, 4, 2);
		ProceduralActor->SetProceduralAsset(Asset);
		ProceduralActor->FinishSpawning(FTransform::Identity);
		ProceduralActor->ApplyWorldOffsetPosition();

		ProceduralActor->SpawnChunks(FVector::ZeroVector, 6);
		
		/*const FDigumWorldProceduralSectionCoordinate Coordinate = ProceduralActor->GetSectionCoordinate(FVector::ZeroVector);
		
		FDigumWorldProceduralSection Section;
		if(ProceduralActor->GetSection(Coordinate.X, Coordinate.Y, Section))
		{
			ProceduralActor->CreateSection(Section);
		}*/
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
		// InComponent->GetOnCoordinateChangedDelegate().AddUObject(this, &ADigumGamePrimaryGameMode::HandleCharacterCoordinateChanged);
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

void ADigumGamePrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

