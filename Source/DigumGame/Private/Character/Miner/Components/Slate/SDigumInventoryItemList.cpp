// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SDigumInventoryItemList.h"

#include "SDigumInventoryItemListSlot.h"
#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SWidget> SDigumInventoryItemList::OnCreateItem(UObject* Item) const
{
	return SNew(SDigumInventoryItemListSlot)
		.Item(Item)
		.Height(100)
		.Width(100);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

