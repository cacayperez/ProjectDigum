// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

class UDigumInventoryComponent;
class SDigumInventoryItemList;
/**
 * 
 */
class DIGUMGAME_API SDigumInventoryWindow : public SDigumWindow
{
public:
	SLATE_BEGIN_ARGS(SDigumInventoryWindow){}
	SLATE_ATTRIBUTE(UDigumInventoryComponent*, InventoryComponent)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:

	
	TWeakObjectPtr<UDigumInventoryComponent> InventoryComponent;
	TArray<UObject*> GetInventoryItems() const;
	virtual TSharedPtr<SWidget> OnCreateContent() override;
	TSharedPtr<SDigumInventoryItemList> ItemList;
	
public:
	void SetInventoryComponent(UDigumInventoryComponent* Component);
	
	
};
