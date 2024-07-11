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

void ADigumGamePrimaryGameMode::OnMapInitialized()
{
	bMapInitialized = true;
	OnGameWorldLoaded.Broadcast();
}

void ADigumGamePrimaryGameMode::StartPlay()
{
	Super::StartPlay();

	
	if(TSubclassOf<ADigumWorldMapActor> WorldMapActorClass = UDigumAssetManager::GetSubclass<ADigumWorldMapActor>(SoftWorldMapActorClass))
	{
		AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), WorldMapActorClass);
		if(ADigumWorldMapActor* MapActor = Cast<ADigumWorldMapActor>(Actor))
		{
			MapActor->SetOwner(this);
			WorldMapActor = MapActor;
			// UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameMode::StartPlay: Found World Map Actor, Owner: %s"), *GetOwner()->GetName());
			WorldMapActor->GetOnWorldLoadedDelegate().AddUObject(this, &ADigumGamePrimaryGameMode::OnMapInitialized);
			WorldMapActor->BeginInitializeMap();
			
		}
	}
}

void ADigumGamePrimaryGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
	UE_LOG(LogTemp, Warning, TEXT("======== GameMode: Handle Player"));
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	// this function doesnt spawn the character yet
}

void ADigumGamePrimaryGameMode::SpawnPlayerCharacter(APlayerController* NewPlayer, TArray<FVector>& StartPositions)
{
	
	if(NewPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Player found"));
		return;
	}

	if(ADigumMinerPlayerController* MinerPlayerController = Cast<ADigumMinerPlayerController>(NewPlayer))
	{
		if(MinerPlayerController->IsCharacterInitialized())
		{
			UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameMode::SpawnPlayerCharacter, Character already initialized"));
			return;
		}
		
		if(TSubclassOf<ADigumMinerCharacter> PlayerCharacterClass = UDigumAssetManager::GetSubclass<ADigumMinerCharacter>(SoftPlayerCharacterClass))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FVector InitialLocation = FVector(0.0f, 0.0f, 200.0f);
			FVector SpawnLocation;

			FindBestStartPosition(InitialLocation, StartPositions, SpawnLocation, 100.0f);
			
			FRotator SpawnRotation = FRotator::ZeroRotator;
			if(ADigumMinerCharacter* Character = GetWorld()->SpawnActor<ADigumMinerCharacter>(PlayerCharacterClass, SpawnLocation, SpawnRotation, SpawnParams))
			{
				UE_LOG(LogTemp, Warning, TEXT("====== Post Login: Spawning character"));
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

void ADigumGamePrimaryGameMode::FindBestStartPosition(const FVector& InInitialPosition,
	TArray<FVector>& OccupiedPositions, FVector& OutPosition, const float& InMinDistance)
{

	bool bFoundPosition = false;
	float CurrentX = InInitialPosition.X;
	float CurrentZ = InInitialPosition.Z;

	if(OccupiedPositions.Num() == 0)
	{
		OutPosition = InInitialPosition;
		OccupiedPositions.Add(OutPosition);
		return;
	}
	
	while(!bFoundPosition)
	{
		CurrentX += InMinDistance;
		CurrentZ += InMinDistance;

		const FVector NewPosition = FVector(CurrentX, InInitialPosition.Y,CurrentZ);

		for(const auto& Position: OccupiedPositions)
		{
			if(Position.Equals(NewPosition, InMinDistance))
			{
				continue;
			}
			else
			{
				OutPosition = NewPosition;
				OccupiedPositions.Add(OutPosition);
				bFoundPosition = true;

				return;
			}
		}
		
	}
	
}

void ADigumGamePrimaryGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	OnGameWorldLoaded.AddLambda([this, NewPlayer]
	{
		SpawnPlayerCharacter(NewPlayer, PlayerStartPositions);
	});	

	
}

