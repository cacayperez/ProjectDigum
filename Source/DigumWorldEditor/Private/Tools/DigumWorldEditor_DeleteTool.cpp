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
	const FDigumWorldAssetCoordinateArray Selection = InParams.Selection;
	if(GEditor && Asset)
	{
		FDigumWorldAssetLayer* Layer = Asset->GetLayer(ActiveLayerIndex);
		if(Layer->IsVisible())
		{
			GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: RemoveCoordinate"));
			Asset->Modify();
			for(const auto SelectionCoordinate : Selection.Coordinates)
			{
				Layer->RemoveCoordinate(SelectionCoordinate.X, SelectionCoordinate.Y);
			}
			Asset->EditorRefresh();
			GEditor->EndTransaction();
		}
	}
}

FText UDigumWorldEditor_DeleteTool::GetToolName() const
{
	return FText::FromString("Delete");
}
