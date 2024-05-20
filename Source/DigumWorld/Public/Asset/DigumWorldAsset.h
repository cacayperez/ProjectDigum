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
	FDigumWorldAssetCoordinate(): X(0), Y(0), SwatchName(NAME_None)
	{
	}

	FDigumWorldAssetCoordinate(int32 InX, int32 InY, FName InSwatchName)
	{
		X = InX;
		Y = InY;
		SwatchName = InSwatchName;
	}
	
	UPROPERTY(EditAnywhere)
	int32 X;

	UPROPERTY(EditAnywhere)
	int32 Y;

	UPROPERTY(EditAnywhere)
	FName SwatchName;

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
	
	UPROPERTY(EditAnywhere)
	TArray<FDigumWorldAssetCoordinate> Coordinates;

	UPROPERTY()
	bool bIsVisible = true;
	
public:
	TArray<FDigumWorldAssetCoordinate>& GetAllCoordinates();
	void AddCoordinate(FDigumWorldAssetCoordinate InCoordinate)
	{
		if(bIsVisible == false)
		{
			return;
		}
		if(HasCoordinate(InCoordinate.X, InCoordinate.Y))
		{
			return;
		}
		
		Coordinates.Add(InCoordinate);
	}

	FText GetLayerName() const { return LayerName; }
	
	bool HasCoordinate(const int32& InX, const int32 InY) const
	{
		const FDigumWorldAssetCoordinate* Coordinate =  Coordinates.FindByPredicate([&InX, &InY](const FDigumWorldAssetCoordinate& Coordinate)
		{
			return InX == Coordinate.X && InY == Coordinate.Y;
		});

		return Coordinate != nullptr;
	}

	bool IsVisible() const { return bIsVisible; }
	void SetVisibility(const bool InVisibility) { bIsVisible = InVisibility; }
	void SetLayerName(const FText& InText) { LayerName = InText; }
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
	TArray<FDigumWorldSwatchPaletteItem>& GetSwatches() { return Swatches; }
	TArray<FDigumWorldAssetCoordinate>& GetCoordinates(const int32& InLayerIndex);
	bool GetCoordinate(const int32& InLayerIndex, const int32& InX, const int32& InY, FDigumWorldAssetCoordinate*& Out);
	FDigumWorldSwatchPaletteItem* GetSwatch(const int32& InIndex);
	FDigumWorldSwatchPaletteItem* GetSwatch(const FName& Name);
	FDigumWorldAssetLayer* GetLayer(const int32& InIndex);
	void RemoveLayer(const int32& InIndex);
	int32 GetWidth() const { return Width; }
	int32 GetHeight() const { return Height; }
	void DeleteLayer(int32 InIndex);
	void SetLayerName(const int32& InLayerIndex, FText& InLayerName);
	void SetLayerVisibility(const int32& InLayerIndex, const bool& bInVisibility);

#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnDigumWorldAssetUpdated);

	FOnDigumWorldAssetUpdated OnDigumWorldAssetUpdated;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditUndo() override;
#endif
};
