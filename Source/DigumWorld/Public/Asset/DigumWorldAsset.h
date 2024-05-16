// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldAsset.generated.h"

class UDigumWorldSwatch;

USTRUCT()
struct FDigumWorldAssetSwatch
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 X;

	UPROPERTY()
	int32 Y;

	bool operator==(const FDigumWorldAssetSwatch& InSwatch) const
	{
		return InSwatch.X == X && InSwatch.Y == Y;
	}
};

USTRUCT()
struct FDigumWorldSwatchPaletteItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName SwatchName;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumWorldSwatch> SoftSwatchAsset;

	bool operator==(const FDigumWorldSwatchPaletteItem& InSwatchPallette) const
	{
		return InSwatchPallette.SwatchName == SwatchName;
	}
};

USTRUCT()
struct FDigumWorldAssetLayer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText LayerName;
	
	UPROPERTY()
	TArray<FDigumWorldAssetSwatch> Swatches;
public:


	void AddSwatch(FDigumWorldAssetSwatch InSwatch)
	{
		Swatches.Add(InSwatch);
	}

	FText GetLayerName() const { return LayerName; }
};



/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldSwatchPaletteItem> Swatches;
	
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldAssetLayer> Layers;

	UPROPERTY(EditAnywhere)
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Height;
	
public:
	void AddSwatchPaletteItem(FDigumWorldSwatchPaletteItem InSwatch);
	void AddNewLayer();
	void UpdateLayer(int32 InIndex, const FDigumWorldAssetLayer& InLayer);
	
	TArray<FDigumWorldAssetLayer>& GetLayers() { return Layers; }
	TArray<FDigumWorldSwatchPaletteItem> GetSwatches() { return Swatches; }
	
	FDigumWorldAssetLayer* GetLayer(int InIndex);
	void RemoveLayer(const int32& InIndex);

#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnDigumWorldAssetUpdated);

	FOnDigumWorldAssetUpdated OnDigumWorldAssetUpdated;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditUndo() override;
#endif
};
