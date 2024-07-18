// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Equip/DigumGameItemActor_Block.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Procedural/DigumWorldMap.h"
#include "GameMode/DigumGameMode.h"
#include "DigumGamePrimaryGameMode.generated.h"

class ADigumWorldMapActor;
struct FDigumWorldProceduralSectionCoordinate;
class ADigumMinerCharacter;
class UDigumWorldMapHandler;
class UDigumWorldProceduralAsset;
class ADigumWorldDynamicProceduralActor;
class UDigumWorldMapAsyncLoader;
UENUM()
enum DigumGamePhase : uint8
{
	GamePhase_None,
	GamePhase_Initializing,
	GamePhase_Playing,
	GamePhase_Ending,
	GamePhase_Paused
};

/**
 * 
 */
UCLASS()
class DIGUMGAME_API ADigumGamePrimaryGameMode : public ADigumGameMode
{
	GENERATED_BODY()
	// We will only use positive seeds
	// Throw error if is on left side of number plane
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	int32 WorldSeed = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UDigumWorldProceduralAsset> ProceduralAsset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	TSoftClassPtr<ADigumWorldMapActor> SoftWorldMapActorClass;

	UPROPERTY()
	bool bMapInitialized = false;

	/*UPROPERTY()
	TObjectPtr<ADigumWorldMapActor> WorldMapActor;*/

	UPROPERTY()
	TArray<FVector> PlayerStartPositions;

public:
	ADigumGamePrimaryGameMode();
	
	virtual void InitGameState() override;
	virtual void PostInitializeComponents() override;
	void OnMapInitialized();
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void SpawnPlayerCharacter(APlayerController* NewPlayer, TArray<FVector>& StartPositions);
	void TryExecuteRequest(const FDigumWorldRequestParams& InParams);

protected:
	FTimerHandle SpawnQueueTimerHandle;
	
	TQueue<APlayerController*> PlayerQueue;

	DECLARE_MULTICAST_DELEGATE(FOnGameWorldLoaded);

	FOnGameWorldLoaded OnGameWorldLoaded;
	
	UPROPERTY()
	float PlayerSpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ADigumMinerCharacter> SoftPlayerCharacterClass;
	
	int32 GetWorldSeed() const;
	void SetWorldSeed(const int32& InValue);
	FVector GetGridSize() const;

	void CheckPlayerSpawnQueue();
	bool SpawnPLayerFromQueue(APlayerController* PlayerController);
	virtual void Tick(float DeltaSeconds) override;

	virtual void FindBestStartPosition(const FVector& InInitialPosition, TArray<FVector>& OccupiedPositions, FVector& OutPosition, const float& InMinDistance = 100.0f);
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnMapLoaded(APlayerController* NewPlayer);
};
