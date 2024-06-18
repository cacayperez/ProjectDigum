// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldProceduralAsset.generated.h"

class UDigumWorldSwatchAsset;


UENUM()
enum EDigumWorldSurfaceAttachType : uint8
{
	DigumWorldAttach_Default,
	DigumWorldAttach_Top,
	DigumWorldAttach_Bottom,
	DigumWorldAttach_Left,
	DigumWorldAttach_Right,
	DigumWorldAttach_All,
};

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
	bool bIsBlocking = true;
	
	UPROPERTY(EditAnywhere)
	int32 Width = 0;

	UPROPERTY(EditAnywhere)
	int32 Height = 0;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDigumWorldSurfaceAttachType> AttachmentType = EDigumWorldSurfaceAttachType::DigumWorldAttach_Default;
	
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldProceduralBlockVariant> Variants;

	TArray<FDigumWorldProceduralBlockVariant> GetVariants() const { return Variants; }

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
	
	static const FDigumWorldProceduralBlock* GetBlockInfo(const FName& InBlockID, const TArray<FDigumWorldProceduralBlock>& InBlocks)
	{
		for(int32 i = 0; i < InBlocks.Num(); i++)
		{
			const FDigumWorldProceduralBlock* Block = &InBlocks[i];
			if(Block->BlockID == InBlockID) return Block;
		}

		return nullptr;
	}
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
