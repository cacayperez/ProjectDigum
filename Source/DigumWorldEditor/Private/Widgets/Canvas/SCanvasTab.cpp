// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SCanvasSelectionLayer.h"
#include "SCanvasView.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"
#include "Selector/DigumWorldEditorSelector.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	bCanSupportFocus = true;
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

void SCanvasTab::OnSelectCanvasCoordinate(const int32& InX, const int32& InY)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->AddSelection(InX, InY);
		// ToolkitPtr.Pin()->AddCoordinateToActiveLayer(InX, InY);
		// ToolkitPtr.Pin()->CallToolAction(InX, InY);
	}
}

void SCanvasTab::OnSetZoomFactor(const float& InZoomValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom Factor: %f"), InZoomValue);
	if(ToolkitPtr.IsValid())
	{
		ZoomFactor = InZoomValue;
		ToolkitPtr.Pin()->SetZoomFactor(InZoomValue);
	}
	
}

void SCanvasTab::OnBeginSelection()
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->BeginSelection();
	}
}

void SCanvasTab::OnEndSelection()
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->EndSelection();
		
	}
}

void SCanvasTab::DrawTab()
{
	_Container->ClearChildren();
	
	if(ToolkitPtr.IsValid())
	{
		UDigumWorldAsset* Asset = GetAsset();

		TSharedPtr<SCanvasView> CanvasView = SNew(SCanvasView)
					.CanvasHeight(Asset->GetHeight())
					.CanvasWidth(Asset->GetWidth())
					.Asset(ToolkitPtr.Pin()->GetAssetBeingEdited())
					.Toolkit(ToolkitPtr.Pin())
					.ZoomFactor(ZoomFactor);

		CanvasView->OnSelectCanvasCoordinate.AddSP(this, &SCanvasTab::OnSelectCanvasCoordinate);
		CanvasView->OnBeginSelection.AddSP(this, &SCanvasTab::OnBeginSelection);
		CanvasView->OnEndSelection.AddSP(this, &SCanvasTab::OnEndSelection);
		CanvasView->OnSetZoomFactor.AddSP(this, &SCanvasTab::OnSetZoomFactor);

		TSharedPtr<SCanvasSelectionLayer> SelectionLayer = SNew(SCanvasSelectionLayer)
					.Toolkit(ToolkitPtr.Pin());
		
		if(Asset)
		{
			_Container->AddSlot()
			[
				SNew(SBox)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					CanvasView.ToSharedRef()
				]
			];

			_Container->AddSlot()
			[
				SNew(SBox)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SelectionLayer.ToSharedRef()
				]
			];
		}
	}
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
