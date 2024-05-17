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
class DIGUMWORLDEDITOR_API SLayerTab : public SBaseTab
{
public:
	SLATE_BEGIN_ARGS(SLayerTab)
		{
		}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
protected:
	TSharedPtr<SWidget> OnCreateLayerMenu();
	void OnLayerNameCommited(const FText& Text, ETextCommit::Type Arg, int InIndex);
	TSharedPtr<SWidget> OnCreateLayerList();

	void AddNewLayer();

	int32 ActiveLayerIndex = 0;
	
public:
	virtual void DrawTab() override;
	
};
