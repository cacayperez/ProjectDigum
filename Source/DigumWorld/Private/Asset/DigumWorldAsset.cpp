// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumWorldAsset.h"

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
		Layers[InIndex] = InLayer;
	}
	
}

FDigumWorldAssetLayer* UDigumWorldAsset::GetLayer(int InIndex)
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

void UDigumWorldAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UDigumWorldAsset::PostEditUndo()
{
	Super::PostEditUndo();
}
