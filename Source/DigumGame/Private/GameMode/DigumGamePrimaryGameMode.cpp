// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameMode/DigumGamePrimaryGameMode.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
#include "Asset/DigumAssetManager.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "GameState/DigumGamePrimaryGameState.h"
#include "Interface/IDigumWorldPawnInterface.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"
#include "Procedural/DigumWorldMapHandler.h"
#include "Procedural/DigumWorldMapLoader.h"
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

void ADigumGamePrimaryGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	UDigumWorldProceduralAsset* Asset = UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);
	
	/*Loader = NewObject<UDigumWorldMapAsyncLoader>(this);
	if(Loader && Asset)
	{
		Loader->CreateMap(Asset, TEXT("Hello World"), GetGridSize(), 12, 12, 1, 1, 3);
	
	}*/

	if(Asset)
	{
		const FVector GridSize = GetGridSize();
		ProceduralActor = GetWorld()->SpawnActorDeferred<ADigumWorldDynamicProceduralActor>(ADigumWorldDynamicProceduralActor::StaticClass(), FTransform::Identity);
		ProceduralActor->GenerateMap(TEXT("Hello World"), GridSize,8, 8, 512, 512, 3, Asset);
		ProceduralActor->FinishSpawning(FTransform::Identity);
		ProceduralActor->ApplyWorldOffsetPosition();
		ProceduralActor->SpawnChunks(FVector::Zero(), 4);
		
	}
}

void ADigumGamePrimaryGameMode::StartPlay()
{
	Super::StartPlay();

	// bInPlayMode = true;
	// ProceduralActor->SpawnChunks(FVector::ZeroVector, 1);
}

void ADigumGamePrimaryGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// this function doesnt spawn the character yet
}

void ADigumGamePrimaryGameMode::HandleCharacterCoordinateChanged(const AActor* Actor,
	const FDigumWorldProceduralSectionCoordinate& InCurrentCoordinate,
	const FDigumWorldProceduralSectionCoordinate& InPreviusCoordinate)
{
	if(Actor)
	{
		ProceduralActor->SetActiveLocation(Actor->GetActorLocation());
	}
}

void ADigumGamePrimaryGameMode::RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent)
{
	if(ProceduralActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Procedural Actor is null"));
		return;
	}
	const FDigumWorldMap* Map = ProceduralActor->GetMap();

	if(Map == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map is null"));
		return;
	}
	
	if(InComponent)
	{
		InComponent->InitializePositioningComponent(Map->GridSize, Map->SectionWidth, Map->SectionHeight, Map->GetWorldOffset());
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

void ADigumGamePrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

