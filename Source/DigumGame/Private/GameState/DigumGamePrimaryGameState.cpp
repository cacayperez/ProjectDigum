// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameState/DigumGamePrimaryGameState.h"

#include "Actor/DigumWorldMapActor.h"
#include "Asset/DigumAssetManager.h"
#include "GameMode/DigumGamePrimaryGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ADigumGamePrimaryGameState::TryInitializeWorld(TSoftClassPtr<ADigumWorldMapActor> InMapActorClass)
{
	InitializeWorld_Internal(InMapActorClass);
}

void ADigumGamePrimaryGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumGamePrimaryGameState, MapActor);
}

void ADigumGamePrimaryGameState::OnMapInitialized()
{
	OnGameWorldLoaded.Broadcast();
}

void ADigumGamePrimaryGameState::InitializeWorld_Internal(TSoftClassPtr<ADigumWorldMapActor> InMapActorClass)
{
	if(TSubclassOf<ADigumWorldMapActor> WorldMapActorClass = UDigumAssetManager::GetSubclass<ADigumWorldMapActor>(InMapActorClass))
	{
		AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), WorldMapActorClass);
		if(ADigumWorldMapActor* WorldMap = Cast<ADigumWorldMapActor>(Actor))
		{
			MapActor = WorldMap;
			MapActor->GetOnWorldLoadedDelegate().AddUObject(this, &ADigumGamePrimaryGameState::OnMapInitialized);
		}
	}
}

void ADigumGamePrimaryGameState::TryExecuteRequest(const FDigumWorldRequestParams& InParams)
{
	if(MapActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameState::TryExecuteRequest"));
		MapActor->TryExecuteAction(InParams);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameState::TryExecuteRequest: MapActor is null"));
	
	}
	/*if(AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		if(ADigumGamePrimaryGameMode* PrimaryGameMode = Cast<ADigumGamePrimaryGameMode>(GameMode))
		{
			UE_LOG(LogTemp, Warning, TEXT("ADigumGamePrimaryGameState::TryExecuteRequest"));
			PrimaryGameMode->TryExecuteRequest(InParams);
		}
	}*/
}
	
