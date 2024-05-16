// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UDigumWorldAsset;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SLayerTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLayerTab)
		{
		}
	SLATE_ATTRIBUTE(UDigumWorldAsset*, AssetBeingEdited)
	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	TSharedPtr<SOverlay> _Container;
	TAttribute<UDigumWorldAsset*> AssetBeingEditedAttribute;
	TSharedPtr<SWidget> OnCreateLayerMenu();
	void OnLayerNameCommited(const FText& Text, ETextCommit::Type Arg, int InIndex);
	TSharedPtr<SWidget> OnCreateLayerList();

	void AddNewLayer();
	void Refresh();
	void DrawTab();
	
};
