// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldDynamicProceduralActor.generated.h"

struct FDigumWorldPositioningParams;
class UDigumWorldPositioningComponent;
class UDigumWorldMapLoaderComponent;
class UDigumActorPool;
struct FDigumWorldProceduralSectionCoordinate;

/*USTRUCT()
struct FDigumPositioningParams
{
	GENERATED_BODY()
public:
	FDigumPositioningParams(const AActor* InPositioningActor)
	{
		PositioningActor = InPositioningActor;
	}
	UPROPERTY()
	const AActor* PositioningActor;
};*/

UCLASS()
class DIGUMWORLD_API ADigumWorldDynamicProceduralActor : public ADigumWorldProceduralActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumWorldMapLoaderComponent> WorldMapLoaderComponent;

	UPROPERTY()
	TArray<FDigumWorldProceduralSectionCoordinate> ActiveCoordinates;

	UPROPERTY()
	TObjectPtr<UDigumActorPool> SectionPool;
	
	UPROPERTY()
	TArray<ADigumWorldActorSection*> InactiveSectionPool;
	
	UPROPERTY()
	int32 PoolSize = 10;
	
	UPROPERTY()
	FDigumWorldProceduralSectionCoordinate ActiveCoordinate;

	UPROPERTY(EditAnywhere)
	FDigumWorldProceduralRules ProceduralRules;

	UPROPERTY()
	TArray<FDigumWorldPositioningParams> PositioningParamsArray;


public:
	// Sets default values for this actor's properties
	ADigumWorldDynamicProceduralActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeDefaultSections();
	virtual bool AddSection_Internal(FDigumWorldProceduralSection& InSection, const bool& bNew = false);
	virtual void OnGenerateMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset);


	UFUNCTION(Unreliable, Server)
	void Server_HandleCoordinateChanged(const FDigumWorldPositioningParams& InParams);

	UFUNCTION(Unreliable, NetMulticast)
	void Multicast_HandleCoordinateChanged(const FDigumWorldPositioningParams& InParams);

	UFUNCTION()
	void HandleCoordinateChanged_Internal(const FDigumWorldPositioningParams& InParams);

	void UpdateCoordinates();

	UFUNCTION(Unreliable, Server)
	void Server_SpawnChunks(const int32& InHalfSize);

	UFUNCTION(Unreliable, NetMulticast)
	void Multicast_SpawnChunks(const int32& InHalfSize);
	
	UFUNCTION()
	void SpawnChunks_Internal(const int32& InHalfSize);

	UFUNCTION(Unreliable, Server)
	void Server_SpawnChunksAtLocation(const FVector& InWorldLocation, const int32& InHalfSize);

	UFUNCTION(Unreliable, NetMulticast)
	void Multicast_SpawnChunksAtLocation(const FVector& InWorldLocation, const int32& InHalfSize);

	UFUNCTION()
	void SpawnChunksAtLocation_Internal(const FVector& InWorldLocation, const int32& InHalfSize);

public:
	void GenerateMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset);

	// virtual ADigumWorldActorSection* GetSectionActor(const int32& InX, const int32& InY) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool TryAddSection(FDigumWorldProceduralSection& InSectionToAdd);
	// virtual void CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* InProceduralAsset) override;
	virtual void AddBlock(const FName& InBlockID, const FVector& InBlockLocation) override;
	TArray<FDigumWorldProceduralSectionCoordinate> GetSectionCoordinatesInRect(const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin, const int32& XMax, const int32& YMin, const int32& YMax) const;
	void SetActiveLocation(const FVector& InLocation);
	void SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize);
	void SpawnChunks(const FDigumWorldProceduralSectionCoordinate& InCoordinate, const int32& InHalfSize);
	void SpawnChunks(const int32& HalfSize);

	
	TArray<ADigumWorldActorSection*> GetInactiveActorPool() const { return InactiveSectionPool; }
	// TArray<ADigumWorldActorSection*> GetActiveActorPool() const { return ActiveSectionPool; }
	
	void InitializePool(int32 InPoolSize, const FName& InFolderPath = NAME_None);
	
	bool SpawnSectionFromPool(const FVector& InLocation, const FRotator& InRotation, FDigumWorldProceduralSection& InSection);
	void HandleCharacterCoordinateChanged(const FDigumWorldPositioningParams& InParams);
	void RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent);
	// void DespawnActorFromPool(ADigumWorldActorSection* InSection);

	void RemoveSection(ADigumWorldActorSection* InSection);
	// void OnInitializeOwner(FDigumWorldPositioningParams& InParams);

	FORCEINLINE UDigumWorldMapLoaderComponent* GetWorldMapLoaderComponent() const { return WorldMapLoaderComponent; }
	FDigumWorldProceduralDefinition* GetProceduralDefinition() const;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent);

};
