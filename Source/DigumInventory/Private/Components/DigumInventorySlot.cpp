// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumInventorySlot.h"

#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
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

void UDigumInventorySlot::SwapContent(UDigumInventorySlot* InOtherSlot)
{
	const FDigumInventoryItemProperties ThisProperties = ItemProperties;
	const FDigumInventoryItemProperties OtherProperties = InOtherSlot->ItemProperties;

	InOtherSlot->SetItemProperties(ThisProperties);
	SetItemProperties(OtherProperties);
}
