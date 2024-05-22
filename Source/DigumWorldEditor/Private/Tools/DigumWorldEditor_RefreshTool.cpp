// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditor_RefreshTool.h"

#include "Asset/DigumWorldAsset.h"

void UDigumWorldEditor_RefreshTool::OnActivateTool(const FDigumWorldEditorToolParams& InParams)
{
	Super::OnActivateTool(InParams);
	
	UDigumWorldAsset* Asset = InParams.Asset;
	const int32 ActiveLayerIndex = InParams.LayerIndex;
	const int32 ActiveSwatchIndex = InParams.SwatchINdex;
	const int32 X = InParams.X;
	const int32 Y = InParams.Y;
	if(GEditor && Asset)
	{
		CleanUpSwatches(Asset);
		CleanUpLayers(Asset);
		CleanUpCoordinates(Asset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset is null cl up>>>>>"));
	}
}

FText UDigumWorldEditor_RefreshTool::GetToolName() const
{
	return Super::GetToolName();
}

TSharedPtr<SWidget> UDigumWorldEditor_RefreshTool::CreateToolWidget()
{
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Clean Up"))
		.OnClicked_Lambda([this]()
		{
			OnSelectToolDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}

void UDigumWorldEditor_RefreshTool::CleanUpSwatches(UDigumWorldAsset* Asset)
{
	// TODO
}

void UDigumWorldEditor_RefreshTool::CleanUpLayers(UDigumWorldAsset* Asset)
{
	// TODO
	
}

void UDigumWorldEditor_RefreshTool::CleanUpCoordinates(UDigumWorldAsset* Asset)
{
	UE_LOG(LogTemp, Warning, TEXT("Clean up>>>>>"));
	// TODO
	for(int32 i = 0; i < Asset->GetLayerCount(); i++)
	{
		FDigumWorldAssetLayer* Layer = Asset->GetLayer(i);
		TArray<FDigumWorldAssetCoordinate> Coordinates = Asset->GetCoordinates(i);
		for(int32 c = 0;  c < Coordinates.Num(); c++)
		{
			FDigumWorldAssetCoordinate* Coordinate = &Coordinates[c];
			if(Coordinate != nullptr)
			{
				const FName SwatchName = Coordinate->SwatchName;
				FDigumWorldSwatchPaletteItem* Swatch = Asset->GetSwatch(SwatchName);
				if(Swatch == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Swatch is null"));
					Layer->RemoveCoordinate(c);
				}
			}
		}
	}
}