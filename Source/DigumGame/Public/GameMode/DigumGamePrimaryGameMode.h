// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Procedural/DigumWorldMap.h"
#include "GameMode/DigumGameMode.h"
#include "DigumGamePrimaryGameMode.generated.h"

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

	UPROPERTY()
	TObjectPtr<ADigumWorldDynamicProceduralActor> ProceduralActor;

	UPROPERTY()
	bool bInPlayMode = false;
public:
	ADigumGamePrimaryGameMode();
	
	virtual void InitGameState() override;
	virtual void PostInitializeComponents() override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void SpawnPlayerCharacter(APlayerController* NewPlayer);
	/*
	void HandleCharacterCoordinateChanged(const AActor* Actor, const FDigumWorldProceduralSectionCoordinate& InCurrentCoordinate, const FDigumWorldProceduralSectionCoordinate& InPreviousCoordinate);
	void RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent);
	*/

protected:
	UPROPERTY()
	float PlayerSpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ADigumMinerCharacter> SoftPlayerCharacterClass;
	
	int32 GetWorldSeed() const;
	void SetWorldSeed(const int32& InValue);
	FVector GetGridSize() const;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void InitializeProceduralMap(APlayerController* NewPlayer);

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnMapLoaded(APlayerController* NewPlayer);
	// void HandleCharacterCoordinateChanged(const AActor* InActor, const FDigumWorldProceduralSectionCoordinate& InPreviousCoordinate, const FDigumWorldProceduralSectionCoordinate& InCurrentCoordinate);
};
