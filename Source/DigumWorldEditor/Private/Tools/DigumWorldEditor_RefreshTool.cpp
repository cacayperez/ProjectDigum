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
	// TODO
}