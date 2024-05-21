// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SBaseTab.h"

/**
 * 
 */
class DIGUMWORLDEDITOR_API SToolTab : public SBaseTab
{
public:
	SLATE_BEGIN_ARGS(SToolTab)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
	virtual void DrawTab() override;
};
