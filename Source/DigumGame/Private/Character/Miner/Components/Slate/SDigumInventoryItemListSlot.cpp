// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SDigumInventoryItemListSlot.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventoryItemListSlot::OnDrawWidget()
{
	ChildSlot
	[
		CreateWidgetSlot().ToSharedRef()
	];
}

TSharedPtr<SWidget> SDigumInventoryItemListSlot::CreateWidgetSlot()
{
	UDigumInventorySlot* Slot = Cast<UDigumInventorySlot>(ItemAttribute.Get());
	if(Slot != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SDigumInventoryItemList::OnCreateItem"));
		const FName ItemID = Slot->GetItemID();

		return SNew(STextBlock)
			.Text(FText::FromName(ItemID));
		
	}

	return nullptr;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

