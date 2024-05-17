// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SBaseTab.h"

class FDigumWorldEditorToolkit;
class UDigumWorldAsset;

/**
 * 
 */
class DIGUMWORLDEDITOR_API SSwatchTab : public SBaseTab
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
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
	virtual void OnConstruct() override;

protected:
	TSharedPtr<SWindow> NewSwatchWindow;
public:
	virtual void DrawTab() override;
};
