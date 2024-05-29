// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumWorldAsset.h"

TArray<FDigumWorldAssetCoordinate>& FDigumWorldAssetCoordinateArray::GetAllCoordinates()
{
	return Coordinates;
}

FDigumWorldAssetCoordinate* FDigumWorldAssetCoordinateArray::GetAt(int32 InArrayIndex)
{
	return Coordinates.IsValidIndex(InArrayIndex) ? &Coordinates[InArrayIndex] : nullptr;
}

TArray<FDigumWorldAssetCoordinate>& FDigumWorldAssetLayer::GetAllCoordinates()
{
	return CoordinateArray.GetAllCoordinates();
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

#if WITH_EDITOR
	OnDigumWorldAssetUpdated.Broadcast();
#endif
}

void UDigumWorldAsset::AddNewLayer()
{
	FDigumWorldAssetLayer NewLayer;
	NewLayer.LayerName = FText::FromString("New Layer");
	Layers.Add(NewLayer);
#if WITH_EDITOR
	OnDigumWorldAssetUpdated.Broadcast();
#endif
}

void UDigumWorldAsset::UpdateLayer(int32 InIndex, const FDigumWorldAssetLayer& InLayer)
{
	if(Layers.IsValidIndex(InIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("TO: %s"), *InLayer.LayerName.ToString())
		Layers[InIndex].LayerName = InLayer.LayerName;
		Layers[InIndex].bIsVisible = InLayer.bIsVisible;
#if WITH_EDITOR
		OnDigumWorldAssetUpdated.Broadcast();
#endif
	}
}

TArray<FDigumWorldAssetCoordinate>& UDigumWorldAsset::GetCoordinates(const int32& InLayerIndex)
{
	TArray<FDigumWorldAssetCoordinate>* Coordinates = nullptr;
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer) Coordinates = &Layer->GetAllCoordinates();

	return Layer->GetAllCoordinates();
}

void UDigumWorldAsset::AddCoordinate(const int32& InLayerIndex, const int32& InX, const int32& InY,
	const FName& InSwatchName)
{
	const FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(InX, InY, InSwatchName);
	AddCoordinate(InLayerIndex, Coordinate);
}

void UDigumWorldAsset::AddCoordinate(const int32& InLayerIndex, const FDigumWorldAssetCoordinate& InCoordinate)
{
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer)
	{
		Layer->AddCoordinate(InCoordinate);
	}
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
#if WITH_EDITOR
		OnDigumWorldAssetUpdated.Broadcast();
#endif
	}

}

void UDigumWorldAsset::DeleteLayer(int32 InIndex)
{
	if(Layers.IsValidIndex(InIndex))
	{
		Layers.RemoveAt(InIndex);
#if WITH_EDITOR
		OnDigumWorldAssetUpdated.Broadcast();
#endif
	}
}

void UDigumWorldAsset::SetLayerName(const int32& InLayerIndex, FText& InLayerName)
{
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer)
	{
		Layer->SetLayerName(InLayerName);
#if WITH_EDITOR
		OnDigumWorldAssetUpdated.Broadcast();
#endif
	}
}

void UDigumWorldAsset::SetLayerVisibility(const int32& InLayerIndex, const bool& bInVisibility)
{
	FDigumWorldAssetLayer* Layer = GetLayer(InLayerIndex);
	if(Layer)
	{
		Layer->SetVisibility(bInVisibility);
#if WITH_EDITOR
		OnDigumWorldAssetUpdated.Broadcast();
#endif
	}
}

void UDigumWorldAsset::RemoveSwatch(const FDigumWorldSwatchPaletteItem& Swatch)
{
	Swatches.Remove(Swatch);
#if WITH_EDITOR
	OnDigumWorldAssetUpdated.Broadcast();
#endif
	
}

void UDigumWorldAsset::SwapLayers(const int32& InLayerIndexA, const int32& InLayerIndexB, int32& OutEndIndex)
{
	if(InLayerIndexA == InLayerIndexB) return;
	if(InLayerIndexA == INDEX_NONE || InLayerIndexB == INDEX_NONE) return;

	const int32 LayerCount = Layers.Num();
	const int32 StartIndex = InLayerIndexA;
	const int32 EndIndex = FMath::Clamp<int32>(InLayerIndexB, 0, LayerCount - 1);

	GetLayers().Swap(StartIndex, EndIndex);
	OutEndIndex = EndIndex;
}

TArray<FDigumWorldAssetLayer> UDigumWorldAsset::GetOrderLayers()
{
	// TODO
	TArray<int32> Hierarchies = GetHierarchies();

	for(int32 HierarchyIndex : Hierarchies)
	{
		TArray<FDigumWorldAssetLayer> HierarchyLayers;
		for(auto Layer : Layers)
		{
			if(Layer.HierarchyIndex == HierarchyIndex)
			{
				HierarchyLayers.Add(Layer);
			}
		}

	}
	
	return {};
}

TArray<int32> UDigumWorldAsset::GetHierarchies()
{
	TArray<int32> Hierarchies;
	for(auto Layer : Layers)
	{
		int32 Index = Layer.HierarchyIndex;

		if(!Hierarchies.Contains(Index))
		{
			Hierarchies.Add(Index);
		}
	}

	return Hierarchies;
}
#if WITH_EDITOR

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

void UDigumWorldAsset::EditorRefresh()
{
	OnDigumWorldAssetUpdated.Broadcast();
}
#endif