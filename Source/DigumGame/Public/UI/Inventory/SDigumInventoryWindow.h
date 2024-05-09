// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

class UDigumInventoryComponent;
class UDigumInventorySlot;
class SDigumInventorySlot;
/**
 * 
 */
class DIGUMGAME_API SDigumInventoryWindow : public SDigumWindow
{
public:
	 SLATE_BEGIN_ARGS(SDigumInventoryWindow){}
		SLATE_ATTRIBUTE(UDigumInventoryComponent*, InventoryComponent)
		SLATE_ATTRIBUTE(TSharedPtr<SDigumWidgetStack>, ParentContainer)
		SLATE_ATTRIBUTE(float, HeightOverride)
		SLATE_ATTRIBUTE(float, WidthOverride)
	 SLATE_END_ARGS()

	~SDigumInventoryWindow();
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
	virtual void OnReceiveDropPayload(UObject* InPayload) override;
	void BeginDragItem(UDigumInventorySlot* ItemSlot);
	void StopDragItem();

 protected:
	int32 GridWidth = 5;
	int32 GridHeight = 5;
	
	TArray<TSharedRef<SDigumInventorySlot>> InventoryItemSlotsWidgets;
	TWeakObjectPtr<UDigumInventoryComponent> WeakInventoryComponent;
	
	virtual TSharedPtr<SWidget> OnCreateContent() override;
	virtual TSharedPtr<SDigumInventorySlot> CreateWidgetItem(UDigumInventorySlot* Item) const;
	

};
