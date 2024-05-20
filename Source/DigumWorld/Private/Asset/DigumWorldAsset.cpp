// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumWorldAsset.h"

TArray<FDigumWorldAssetCoordinate>& FDigumWorldAssetLayer::GetAllCoordinates()
{
	return Coordinates;
}

void UDigumWorldAsset::AddSwatchPaletteItem(FDigumWorldSwatchPaletteItem InSwatch)
{
	bool bExisting = false;

	for(auto SwatchItem : Swatches)
	{
		if(SwatchItem.SwatchName == InSwatch.SwatchName)
		{
			SwatchItem = InSwatch;
			bExisting = true;
			break;
		}
	}

	if(bExisting == false)
	{
		Swatches.Add(InSwatch);
	}

	OnDigumWorldAssetUpdated.Broadcast();
}

void UDigumWorldAsset::AddNewLayer()
{
	FDigumWorldAssetLayer NewLayer;
	NewLayer.LayerName = FText::FromString("New Layer");
	Layers.Add(NewLayer);
	OnDigumWorldAssetUpdated.Broadcast();
}

void UDigumWorldAsset::UpdateLayer(int32 InIndex, const FDigumWorldAssetLayer& InLayer)
{
	if(Layers.IsValidIndex(InIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("TO: %s"), *InLayer.LayerName.ToString())
		Layers[InIndex].LayerName = InLayer.LayerName;
		Layers[InIndex].bIsVisible = InLayer.bIsVisible;
		OnDigumWorldAssetUpdated.Broadcast();
	}
}

TArray<FDigumWorldAssetCoordinate>& UDigumWorldAsset::GetCoordinates(const int32& InLayerIndex)
{
	TArray<FDigumWorldAssetCoordinate>* Coordinates = nullptr;
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer) Coordinates = &Layer->GetAllCoordinates();

	return Layer->GetAllCoordinates();
}

bool UDigumWorldAsset::GetCoordinate(const int32& InLayerIndex, const int32& InX, const int32& InY, FDigumWorldAssetCoordinate*& OutCoordinate)
{
	// FDigumWorldAssetCoordinate* Coordinate = nullptr;

	for(FDigumWorldAssetCoordinate& C : GetCoordinates(InLayerIndex))
	{
		
		if(C.X == InX && C.Y == InY)
		{
			OutCoordinate = &C;
			return true;
		}
	}

	return false;
}

FDigumWorldSwatchPaletteItem* UDigumWorldAsset::GetSwatch(const int32& InIndex)
{
	return Swatches.IsValidIndex(InIndex) ? &Swatches[InIndex] : nullptr;
}

FDigumWorldSwatchPaletteItem* UDigumWorldAsset::GetSwatch(const FName& InName)
{
	// FDigumWorldSwatchPaletteItem* Item = nullptr;

	FDigumWorldSwatchPaletteItem* SwatchPaletteItem = Swatches.FindByPredicate([&](FDigumWorldSwatchPaletteItem& Item)
	{
		return Item.SwatchName == InName;
	});

	return SwatchPaletteItem;
}


FDigumWorldAssetLayer* UDigumWorldAsset::GetLayer(const int32& InIndex)
{
	return Layers.IsValidIndex(InIndex) ? &Layers[InIndex] : nullptr;
}


void UDigumWorldAsset::RemoveLayer(const int32& InIndex)
{
	if(Layers.IsValidIndex(InIndex))
	{
		Layers.RemoveAt(InIndex);
		OnDigumWorldAssetUpdated.Broadcast();
	}

}

void UDigumWorldAsset::DeleteLayer(int32 InIndex)
{
	if(Layers.IsValidIndex(InIndex))
	{
		Layers.RemoveAt(InIndex);
		OnDigumWorldAssetUpdated.Broadcast();
	}
}

void UDigumWorldAsset::SetLayerName(const int32& InLayerIndex, FText& InLayerName)
{
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer)
	{
		Layer->SetLayerName(InLayerName);
		OnDigumWorldAssetUpdated.Broadcast();
	}
}

void UDigumWorldAsset::SetLayerVisibility(const int32& InLayerIndex, const bool& bInVisibility)
{
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer)
	{
		Layer->SetVisibility(bInVisibility);
		OnDigumWorldAssetUpdated.Broadcast();
	}
}


void UDigumWorldAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnDigumWorldAssetUpdated.Broadcast();
}

void UDigumWorldAsset::PostEditUndo()
{
	Super::PostEditUndo();
	OnDigumWorldAssetUpdated.Broadcast();
}
