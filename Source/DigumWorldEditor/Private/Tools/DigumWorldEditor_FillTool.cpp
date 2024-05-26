// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditor_FillTool.h"

#include "Asset/DigumWorldAsset.h"

struct FDigumWorldSwatchPaletteItem;

void UDigumWorldEditor_FillTool::OnActivateTool(const FDigumWorldEditorToolParams& InParams)
{
	Super::OnActivateTool(InParams);

	UDigumWorldAsset* Asset = InParams.Asset;
	const int32 ActiveLayerIndex = InParams.LayerIndex;
	const int32 ActiveSwatchIndex = InParams.SwatchINdex;
	if(GEditor && Asset)
	{
		const int32 Width = Asset->GetWidth();
		const int32 Height = Asset->GetHeight();
		FDigumWorldSwatchPaletteItem* Swatch = Asset->GetSwatch(ActiveSwatchIndex);
		FDigumWorldAssetLayer* Layer = Asset->GetLayer(ActiveLayerIndex);
		if(Swatch)
		{
			FName SwatchName = Swatch->SwatchName;
		
			GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: RemoveCoordinate"));
			Asset->Modify();
			for(int32 x = 0; x < Width; x++)
			{
				for(int32 y = 0; y < Height; y++)
				{
					FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(x, y, SwatchName);
					Asset->AddCoordinate(ActiveLayerIndex, Coordinate);
				}
			}
			Asset->EditorRefresh();
			GEditor->EndTransaction();
		}
	}
}

FText UDigumWorldEditor_FillTool::GetToolName() const
{
	return Super::GetToolName();
}

TSharedPtr<SWidget> UDigumWorldEditor_FillTool::CreateToolWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Fill"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectToolDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}
