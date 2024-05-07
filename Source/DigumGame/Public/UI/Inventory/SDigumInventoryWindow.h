// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

class UDigumInventoryComponent;
class UDigumInventorySlot;
/**
 * 
 */
class DIGUMGAME_API SDigumInventoryWindow : public SDigumWindow
{
public:
	 SLATE_BEGIN_ARGS(SDigumInventoryWindow){}
		SLATE_ATTRIBUTE(UDigumInventoryComponent*, InventoryComponent)
		SLATE_ATTRIBUTE(float, HeightOverride)
		SLATE_ATTRIBUTE(float, WidthOverride)
	 SLATE_END_ARGS()
		
	void Construct(const FArguments& InArgs);
protected:
	int32 GridWidth = 5;
	int32 GridHeight = 5;
	
	TSharedPtr<SGridPanel> _Container;
	TWeakObjectPtr<UDigumInventoryComponent> WeakInventoryComponent;

	virtual TSharedPtr<SWidget> OnCreateContent() override;
	virtual TSharedPtr<SWidget> CreateWidgetItem(UDigumInventorySlot* Item) const;
};
