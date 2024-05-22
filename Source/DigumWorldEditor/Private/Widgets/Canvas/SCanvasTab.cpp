// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SCanvasView.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}
void SCanvasTab::OnSelectCanvasCoordinate(const int32& InX, const int32& InY)
{
	if(ToolkitPtr.IsValid())
	{
		// ToolkitPtr.Pin()->AddCoordinateToActiveLayer(InX, InY);
		ToolkitPtr.Pin()->CallToolAction(InX, InY);
		UE_LOG(LogTemp, Warning, TEXT("Selected Coordinate: %d, %d"), InX, InY);
		RefreshTab();
	}
}

void SCanvasTab::OnSetZoomFactor(const float& InZoomValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom Factor: %f"), InZoomValue);
	ZoomFactor = InZoomValue;
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
					.ZoomFactor(ZoomFactor);

		CanvasView->OnSelectCanvasCoordinate.AddSP(this, &SCanvasTab::OnSelectCanvasCoordinate);
		CanvasView->OnSetZoomFactor.AddSP(this, &SCanvasTab::OnSetZoomFactor);
		
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
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
