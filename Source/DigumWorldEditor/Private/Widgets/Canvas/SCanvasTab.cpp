// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasTab.h"

#include "SCanvasView.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

int32 SCanvasTab::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

void SCanvasTab::DrawTab()
{
	_Container->ClearChildren();
	
	if(ToolkitPtr.IsValid())
	{
		UDigumWorldAsset* Asset = GetAsset();
		
		if(Asset)
		{
			_Container->AddSlot()
			[
				SNew(SCanvasView)
				.CanvasHeight(Asset->GetHeight())
				.CanvasWidth(Asset->GetWidth())
				.Layers(Asset->GetLayers())
			];
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
