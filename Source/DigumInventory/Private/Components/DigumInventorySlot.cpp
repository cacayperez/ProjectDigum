// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumInventorySlot.h"
#include "Components/DigumInventoryComponent.h"
UDigumInventorySlot::UDigumInventorySlot()
{
}

void UDigumInventorySlot::SetItemProperties(const FDigumInventoryItemProperties& InItemProperties)
{
	if(InItemProperties.IsValid())
	{
		ItemProperties = InItemProperties;
		bEmpty = false;
	}
	else
	{
		ClearItemProperties();
	}
}

void UDigumInventorySlot::ClearItemProperties()
{
	bEmpty = true;
	ItemProperties = FDigumInventoryItemProperties();
}
