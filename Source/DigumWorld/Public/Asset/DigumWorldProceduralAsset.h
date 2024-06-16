// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldProceduralAsset.generated.h"

class UDigumWorldSwatchAsset;

USTRUCT()
struct FDigumWorldProceduralBlockVariant
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName VariantName;

	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;

};

USTRUCT()
struct FDigumWorldProceduralBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName BlockID = NAME_None;

	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlockVariant> Variants;

	TArray<FDigumWorldProceduralBlockVariant> GetVariants() const { return Variants; }

};

USTRUCT()
struct FDigumWorldProceduralBlock_Sized : public FDigumWorldProceduralBlock
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Height;
};

USTRUCT()
struct FDigumWorldProceduralDefinition
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlock> TerrainBlocks;

	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlock> GrassFoliage;

	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlock> TreesFoliage;

	int32 GetTerrainBlockCount() const { return TerrainBlocks.Num(); }
	int32 GetGrassBlockCount() const { return GrassFoliage.Num(); }
	int32 GetTreesBlockCount() const { return TreesFoliage.Num(); }
	TArray<FDigumWorldProceduralBlock> GetTerrainBlocks() const { return TerrainBlocks;}
	TArray<FDigumWorldProceduralBlock> GetGrassBlocks() const { return GrassFoliage;}
	TArray<FDigumWorldProceduralBlock> GetTreesBlock() const { return TreesFoliage;}
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
	FDigumWorldProceduralDefinition ProceduralDefinition;
	
	// UDigumWorldSwatchAsset* GetSwatchAsset(const FName& BlockID);
};
