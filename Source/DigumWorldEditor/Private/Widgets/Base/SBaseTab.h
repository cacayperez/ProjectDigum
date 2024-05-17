// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDigumWorldAsset;
class FDigumWorldEditorToolkit;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SBaseTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseTab)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
	virtual void OnConstruct();
protected:
	TSharedPtr<SOverlay> _Container;
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	UDigumWorldAsset* GetAsset() const;

	virtual void OnAssetUpdated();

public:
	virtual void DrawTab();
	void RefreshTab();
};
