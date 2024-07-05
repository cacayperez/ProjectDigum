// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameMode/DigumGamePrimaryGameMode.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
#include "Actor/DigumWorldMapActor.h"
#include "Asset/DigumAssetManager.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "GameState/DigumGamePrimaryGameState.h"
#include "Interface/IDigumWorldPawnInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DigumMinerPlayerController.h"
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


}

void ADigumGamePrimaryGameMode::StartPlay()
{
	Super::StartPlay();

	/*// TODO Move spawning somewhere else because spawning here doesnt replicate to clients
	UDigumWorldProceduralAsset* Asset = UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);
	
	if(Asset)
	{
		const FVector GridSize = GetGridSize();
		ProceduralActor = GetWorld()->SpawnActorDeferred<ADigumWorldDynamicProceduralActor>(ADigumWorldDynamicProceduralActor::StaticClass(), FTransform::Identity);
		ProceduralActor->SetReplicates(true);
		ProceduralActor->GenerateMap(TEXT("Hello World"), GridSize,8, 8, 512, 512, 2, Asset);
		ProceduralActor->FinishSpawning(FTransform::Identity);
		ProceduralActor->ApplyWorldOffsetPosition();
		ProceduralActor->SpawnChunks(FVector::Zero(), 4);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("======== GameMode: Start Play"));*/
}

void ADigumGamePrimaryGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
	UE_LOG(LogTemp, Warning, TEXT("======== GameMode: Handle Player"));
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	// this function doesnt spawn the character yet
}

void ADigumGamePrimaryGameMode::SpawnPlayerCharacter(APlayerController* NewPlayer)
{
	
	if(NewPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Player found"));
		return;
	}

	if(ADigumMinerPlayerController* MinerPlayerController = Cast<ADigumMinerPlayerController>(NewPlayer))
	{
		if(TSubclassOf<ADigumMinerCharacter> PlayerCharacterClass = UDigumAssetManager::GetSubclass<ADigumMinerCharacter>(SoftPlayerCharacterClass))
		{
			FVector SpawnLocation = FVector(0.0f, 0.0f, 200.0f);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			if(ADigumMinerCharacter* Character = GetWorld()->SpawnActor<ADigumMinerCharacter>(PlayerCharacterClass, SpawnLocation, SpawnRotation))
			{
				UE_LOG(LogTemp, Warning, TEXT("====== Post Login: Spawning character"));
				// NewPlayer->Possess(Character);
				MinerPlayerController->InitializeController(Character);			
			}

		}
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


void ADigumGamePrimaryGameMode::InitializeWorldMap(APlayerController* NewPlayer)
{
	if(NewPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameMode::InitializeWorldMap: No Player found"));
		return;
	}
	/*if(NewPlayer)
	{
		if(ADigumMinerPlayerController* MPC = Cast<ADigumMinerPlayerController>(NewPlayer))
		{
			MPC->TrySpawnWorldMapActor();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameMode::InitializeWorldMap: Player Controller is not of type ADigumMinerPlayerController"));
		}
	}*/

}


void ADigumGamePrimaryGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameMode::PostLogin: %s"), *NewPlayer->GetName());

	InitializeWorldMap(NewPlayer);

	
}

