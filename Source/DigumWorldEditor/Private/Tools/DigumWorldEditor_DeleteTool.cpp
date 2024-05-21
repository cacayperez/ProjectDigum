// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditor_DeleteTool.h"

#include "Asset/DigumWorldAsset.h"

struct FDigumWorldAssetLayer;
struct FDigumWorldSwatchPaletteItem;

TSharedPtr<SWidget> UDigumWorldEditor_DeleteTool::CreateToolWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Delete"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectToolDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}

void UDigumWorldEditor_DeleteTool::OnActivateTool(const FDigumWorldEditorToolParams& InParams)
{
	Super::OnActivateTool(InParams);

	UDigumWorldAsset* Asset = InParams.Asset;
	const int32 ActiveLayerIndex = InParams.LayerIndex;
	const int32 ActiveSwatchIndex = InParams.SwatchINdex;
	const int32 X = InParams.X;
	const int32 Y = InParams.Y;
	if(GEditor && Asset)
	{
		FDigumWorldSwatchPaletteItem* Swatch = Asset->GetSwatch(ActiveSwatchIndex);
		FDigumWorldAssetLayer* Layer = Asset->GetLayer(ActiveLayerIndex);
		if(Swatch && Layer && Layer->IsVisible())
		{
			FName SwatchName = Swatch->SwatchName;
			
			FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(X, Y, SwatchName);
			GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: RemoveCoordinate"));
			Asset->Modify();
			Layer->RemoveCoordinate(X, Y);
			GEditor->EndTransaction();
		}
	}
}

FText UDigumWorldEditor_DeleteTool::GetToolName() const
{
	return FText::FromString("Delete");
}
