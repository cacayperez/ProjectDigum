// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FDigumWorldEditorToolkit;
class UDigumWorldAsset;

/**
 * 
 */
class DIGUMWORLDEDITOR_API SSwatchTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSwatchTab)
		{
		}
	SLATE_ATTRIBUTE(UDigumWorldAsset*, AssetBeingEdited)
	SLATE_END_ARGS()

	FReply OnNewSwatchClicked();
	/** Constructs this widget with InArgs */
	// void Construct(const FArguments& InArgs);
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkitPtr);
protected:
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	TAttribute<UDigumWorldAsset*> AssetBeingEditedAttribute;
	TSharedPtr<SOverlay> _Container;
	
	void DrawTab();
	void Refresh();
};
