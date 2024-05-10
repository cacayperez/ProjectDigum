// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

class UDigumGameInventoryComponent;
class UDigumInventorySlot;
class SDigumInventorySlot;
struct FDigumWindowStyle;
/**
 * 
 */
class DIGUMGAME_API SDigumInventoryWindow : public SDigumWindow
{
public:
	 SLATE_BEGIN_ARGS(SDigumInventoryWindow){}
		SLATE_ATTRIBUTE(UDigumGameInventoryComponent*, InventoryComponent)
		SLATE_ATTRIBUTE(TSharedPtr<SDigumWidgetStack>, ParentContainer)
		SLATE_ATTRIBUTE(UMaterialInterface*, BackgroundMaterial)
		SLATE_ATTRIBUTE(UMaterialInterface*, InventorySlotMaterial)
		SLATE_ATTRIBUTE(FDigumWindowStyle*, WindowStyle);
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
	TAttribute<UMaterialInterface*> InventorySlotMaterialAttribute;
	
	TSharedPtr<SGridPanel> InventoryGridPanel;
	int32 GridWidth = 5;
	int32 GridHeight = 5;
	bool bIsTransactingItem = false;

	TArray<TSharedRef<SDigumInventorySlot>> InventoryItemSlotsWidgets;
	TWeakObjectPtr<UDigumGameInventoryComponent> WeakInventoryComponent;

	virtual TSharedPtr<SWidget> OnCreateContent() override;
	virtual TSharedPtr<SDigumInventorySlot> CreateWidgetItem(UDigumInventorySlot* Item) const;

	void UpdateInventoryGridPanel();

public:
	void SetInventoryComponent(UDigumGameInventoryComponent* InInventoryComponent);
};
