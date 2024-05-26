// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SBaseTab.h"

class SCanvasView;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasTab : public SBaseTab
{
public:
	SLATE_BEGIN_ARGS(SCanvasTab)
		{
		}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
protected:
	
	float ZoomFactor = 1.0f;

public:
	void OnSelectCanvasCoordinate(const int32& InX, const int32& InY);
	void OnSetZoomFactor(const float& InZoomValue);
	void OnBeginSelection();
	void OnEndSelection();
	virtual void DrawTab() override;

	// void RenderGrid();
};
