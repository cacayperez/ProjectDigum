// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldProceduralAsset.generated.h"

class UDigumWorldSwatchAsset;

USTRUCT()
struct FDigumWorldProceduralBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName BlockID = NAME_None;
	
	/*UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;*/

	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;
};

/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldProceduralAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlock> TerrainBlocks;

	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlock> GrassFoliage;

	int32 GetTerrainBlockCount() const { return TerrainBlocks.Num(); }
	int32 GetGrassBlockCount() const { return GrassFoliage.Num(); }
	TArray<FDigumWorldProceduralBlock> GetTerrainBlocks() const { return TerrainBlocks;}
	TArray<FDigumWorldProceduralBlock> GetGrassBlocks() const { return GrassFoliage;}
	
	// UDigumWorldSwatchAsset* GetSwatchAsset(const FName& BlockID);
};
