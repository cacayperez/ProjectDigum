// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/SCompoundWidget.h"

class UDigumWorldEditorSwatch;
class FDigumWorldEditorToolkit;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SNewSwatchWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNewSwatchWindow)
		{
		}
	SLATE_ATTRIBUTE(UDigumWorldEditorSwatch*, Swatch)
	SLATE_END_ARGS()

	FReply OnAddClicked();
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkitPtr);
	
protected:
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	FDigumWorldSwatchPaletteItem SwatchItem;
	TAttribute<UDigumWorldEditorSwatch*> Swatch;
};
