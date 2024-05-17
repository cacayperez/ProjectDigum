// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldAsset.generated.h"

class UDigumWorldSwatchAsset;

USTRUCT()
struct FDigumWorldAssetCoordinate
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 X;

	UPROPERTY()
	int32 Y;

	FName SwatchName = NAME_None;

	bool operator==(const FDigumWorldAssetCoordinate& InCoordinate) const
	{
		return InCoordinate.X == X && InCoordinate.Y == Y;
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
	TSoftObjectPtr<UDigumWorldSwatchAsset> SoftSwatchAsset;

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
	TArray<FDigumWorldAssetCoordinate> Coordinates;

	UPROPERTY()
	bool bIsVisible = true;
public:
	
	void AddCoordinate(FDigumWorldAssetCoordinate InCoordinate)
	{
		Coordinates.Add(InCoordinate);
	}

	FText GetLayerName() const { return LayerName; }
	
	bool HasCoordinate(const FDigumWorldAssetCoordinate& InCoordinate) const
	{
		const FDigumWorldAssetCoordinate* Coordinate =  Coordinates.FindByPredicate([&InCoordinate](const FDigumWorldAssetCoordinate& Coordinate)
		{
			return Coordinate == InCoordinate;
		});

		return Coordinate != nullptr;
	}

	bool IsVisible() const { return bIsVisible; }
	void SetVisibility(const bool InVisibility) { bIsVisible = InVisibility; }
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
	int32 GetWidth() const { return Width; }
	int32 GetHeight() const { return Height; }

#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnDigumWorldAssetUpdated);

	FOnDigumWorldAssetUpdated OnDigumWorldAssetUpdated;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditUndo() override;
#endif
};
