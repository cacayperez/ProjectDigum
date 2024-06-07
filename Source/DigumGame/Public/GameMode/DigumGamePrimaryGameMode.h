// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/DigumGamePlayerCharacter.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "GameFramework/GameMode.h"
#include "DigumGamePrimaryGameMode.generated.h"

struct FDigumWorldProceduralSectionCoordinate;
class UDigumWorldMapHandler;
class UDigumWorldProceduralAsset;
class ADigumWorldDynamicProceduralActor;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API ADigumGamePrimaryGameMode : public AGameMode
{
	GENERATED_BODY()
	// We will only use positive seeds
	// Throw error if is on left side of number plane
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	int32 WorldSeed = -1;

	/*
	UPROPERTY()
	TObjectPtr<UDigumWorldMapHandler> WorldMapHandler;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Digum Game State", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UDigumWorldProceduralAsset> ProceduralAsset;

	UPROPERTY()
	TObjectPtr<ADigumWorldDynamicProceduralActor> ProceduralActor;

public:
	ADigumGamePrimaryGameMode();
	
	virtual void InitGameState() override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent);

protected:
	int32 GetWorldSeed() const;
	void SetWorldSeed(const int32& InValue);
	FVector GetGridSize() const;
	/*FVector2D GetSectionSize() const;
	FVector GetWorldOffset() const*/;
	
	
	virtual void Tick(float DeltaSeconds) override;
	// void HandleCharacterCoordinateChanged(const AActor* InActor, const FDigumWorldProceduralSectionCoordinate& InPreviousCoordinate, const FDigumWorldProceduralSectionCoordinate& InCurrentCoordinate);
};
