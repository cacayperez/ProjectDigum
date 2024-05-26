// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditor_AddTool.h"

#include "Asset/DigumWorldAsset.h"

void UDigumWorldEditor_AddTool::OnActivateTool(const FDigumWorldEditorToolParams& InParams)
{
	Super::OnActivateTool(InParams);

	UDigumWorldAsset* Asset = InParams.Asset;
	const int32 ActiveLayerIndex = InParams.LayerIndex;
	const int32 ActiveSwatchIndex = InParams.SwatchINdex;
	FDigumWorldAssetCoordinateArray Selection = InParams.Selection;
	if(GEditor && Asset)
	{
		FDigumWorldSwatchPaletteItem* Swatch = Asset->GetSwatch(ActiveSwatchIndex);
		FDigumWorldAssetLayer* Layer = Asset->GetLayer(ActiveLayerIndex);
		if(Swatch && Layer && Layer->IsVisible())
		{
			GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: AddCoordinate"));
			Asset->Modify();
			for(const FDigumWorldAssetCoordinate& SelectionCoordinate : Selection.Coordinates)
			{
				Layer->AddCoordinate(SelectionCoordinate);
			
			}
			GEditor->EndTransaction();
		}
	}
}

FText UDigumWorldEditor_AddTool::GetToolName() const
{
	return FText::FromString("Add");
}

TSharedPtr<SWidget> UDigumWorldEditor_AddTool::CreateToolWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Add"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectToolDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}
