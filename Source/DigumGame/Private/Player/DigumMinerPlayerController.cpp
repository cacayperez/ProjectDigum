// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/DigumMinerPlayerController.h"

#include "Actor/Background/DigumGameBackgroundActor.h"
#include "Asset/DigumAssetManager.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/DigumWorldSettings.h"


class UEnhancedInputLocalPlayerSubsystem;
class UDigumGameDeveloperSettings;

// Sets default values
ADigumMinerPlayerController::ADigumMinerPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADigumMinerPlayerController::InitializeController(ADigumMinerCharacter* InCharacter)
{
	if(InCharacter && !bCharacterHasBeenInitialized)
	{
		Possess(InCharacter);
		bCharacterHasBeenInitialized = true;
	}
}

/*
void ADigumMinerPlayerController::TrySpawnPlayerCharacter(const FVector& InWorldLocation)
{
	Server_SpawnPlayerCharacter(InWorldLocation);
}

void ADigumMinerPlayerController::TrySpawnWorldMapActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Spawn World Map Actor %s"), *GetName());
	Server_SpawnWorldMapActor();
}

void ADigumMinerPlayerController::Multicast_SpawnPlayerCharacter_Implementation(const FVector& InWorldLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Multicast Spawn Player Character %s"), *GetName());
	if(HasAuthority())
	{
		SpawnPlayerCharacter_Internal(InWorldLocation);
	}
}

void ADigumMinerPlayerController::Server_SpawnPlayerCharacter_Implementation(const FVector& InWorldLocation)
{
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Spawn Player Character %s"), *GetName());
		Multicast_SpawnPlayerCharacter(InWorldLocation);
	}
}

void ADigumMinerPlayerController::Multicast_SpawnWorldMapActor_Implementation()
{
	if(IsLocalController())
	{
		SpawnWorldMapActor_Internal();

		UE_LOG(LogTemp, Warning, TEXT("Spawned World Map Actor %s"), *GetName());
	}
}

void ADigumMinerPlayerController::Server_SpawnWorldMapActor_Implementation()
{
	if(HasAuthority())
	{
		Multicast_SpawnWorldMapActor();
	}
}
*/

void ADigumMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBackground();
	// TrySpawnWorldMapActor();
	
}

void ADigumMinerPlayerController::InitializeBackground()
{
	if(TSubclassOf<ADigumGameBackgroundActor> BackgroundClass = UDigumAssetManager::GetSubclass<ADigumGameBackgroundActor>(DefaultBackgroundClass))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		
		if(ADigumGameBackgroundActor* BackgroundActor = GetWorld()->SpawnActor<ADigumGameBackgroundActor>(BackgroundClass, Location, Rotation, SpawnParameters))
		{
			BackgroundActor->Initialize(this);
		}
	}
}

/*
void ADigumMinerPlayerController::SpawnPlayerCharacter_Internal(const FVector& InWorldLocation)
{
	if(bCharacterHasBeenInitialized) return;
	
	TSubclassOf<ACharacter> CharacterClass = UDigumAssetManager::GetSubclass<ACharacter>(DefaultPlayerCharacterClass);
	if(CharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if(ACharacter* CharacterToSpawn = GetWorld()->SpawnActor<ACharacter>(CharacterClass, InWorldLocation, FRotator::ZeroRotator, SpawnParams))
		{
			bCharacterHasBeenInitialized = true;
			Possess(CharacterToSpawn);
			OnMinerCharacterSpawned.Broadcast(CharacterToSpawn);
		}
	}
}

void ADigumMinerPlayerController::SpawnWorldMapActor_Internal()
{
	if(TSubclassOf<ADigumWorldMapActor> WorldMapClass = UDigumWorldSettings::GetWorldMapActorClass())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), WorldMapClass);
		if(ADigumWorldMapActor* WorldMapActor = GetWorld()->SpawnActor<ADigumWorldMapActor>(WorldMapClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			// WorldMapActor->SetOwningPlayerController(NewPlayer);
			WorldMapActor->BeginInitializeMap();
			WorldMapActor->GetOnWorldLoadedDelegate().AddLambda([ this]()
			{
				TrySpawnPlayerCharacter(FVector(0, 0 , 200));
			});
		}
	}
}
*/

