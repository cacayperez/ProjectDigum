// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldDynamicProceduralActor.generated.h"

struct FDigumWorldProceduralSectionCoordinate;

UCLASS()
class DIGUMWORLD_API ADigumWorldDynamicProceduralActor : public ADigumWorldProceduralActor
{
	GENERATED_BODY()

	
public:
	// Sets default values for this actor's properties
	ADigumWorldDynamicProceduralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeDefaultSections();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<FDigumWorldProceduralSectionCoordinate> GetSectionCoordinatesInRadius(const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& Radius, const int32& XMin, const int32& XMax, const int32& YMin, const int32 YMax) const;
	void SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize);

};
