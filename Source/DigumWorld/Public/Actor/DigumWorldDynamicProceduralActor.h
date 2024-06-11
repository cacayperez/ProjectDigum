// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldDynamicProceduralActor.generated.h"

class UDigumActorPool;
struct FDigumWorldProceduralSectionCoordinate;

UCLASS()
class DIGUMWORLD_API ADigumWorldDynamicProceduralActor : public ADigumWorldProceduralActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDigumWorldProceduralSectionCoordinate> ActiveCoordinates;

	UPROPERTY()
	TObjectPtr<UDigumActorPool> SectionPool;
	
	UPROPERTY()
	TArray<ADigumWorldActorSection*> InactiveSectionPool;

	UPROPERTY()
	TArray<ADigumWorldActorSection*> ActiveSectionPool;

	UPROPERTY()
	int32 PoolSize = 10;
	
public:
	// Sets default values for this actor's properties
	ADigumWorldDynamicProceduralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeDefaultSections();
	virtual void OnGenerateMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset) override;
public:
	virtual ADigumWorldActorSection* GetSectionActor(const int32& InX, const int32& InY) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void CreateSection(FDigumWorldProceduralSection InSection) override;
	// virtual void CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* InProceduralAsset) override;
	virtual void AddBlock(const FName& InBlockID, const FVector& InBlockLocation) override;
	TArray<FDigumWorldProceduralSectionCoordinate> GetSectionCoordinatesInRect(const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin, const int32& XMax, const int32& YMin, const int32& YMax) const;
	void SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize);
	void SpawnChunks(const FDigumWorldProceduralSectionCoordinate& InCoordinate, const int32& HalfSize);
	void PurgeChunks();

	
	void OnSectionSpawned(AActor * Actor);
	
	TArray<ADigumWorldActorSection*> GetInactiveActorPool() const { return InactiveSectionPool; }
	TArray<ADigumWorldActorSection*> GetActiveActorPool() const { return ActiveSectionPool; }
	
	void InitializePool(int32 InPoolSize, const FName& InFolderPath = NAME_None);
	bool SpawnSectionFromPool(const FVector& InLocation, const FRotator& InRotation, FDigumWorldProceduralSection& InSection);
	void DespawnActorFromPool(ADigumWorldActorSection* InSection);

};
