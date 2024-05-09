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
		Clear();
	}
}

void UDigumInventorySlot::SetItemObject(UDigumItem* InItemObject)
{
	ItemObject = InItemObject;
}

void UDigumInventorySlot::Clear()
{
	bEmpty = true;
	ItemProperties = FDigumInventoryItemProperties();
	ItemObject = nullptr;
}

void UDigumInventorySlot::SwapContent(UDigumInventorySlot* InOtherSlot)
{
	if(InOtherSlot == nullptr)
	{
		return;
	}
	
	const FDigumInventoryItemProperties ThisProperties = ItemProperties;
	const FDigumInventoryItemProperties OtherProperties = InOtherSlot->ItemProperties;
	UDigumItem* ThisItemObject = ItemObject.Get();
	UDigumItem* OtherItemObject = InOtherSlot->GetItemObject();
	
	InOtherSlot->SetItemProperties(ThisProperties);
	InOtherSlot->SetItemObject(ThisItemObject);
	
	SetItemProperties(OtherProperties);
	SetItemObject(OtherItemObject);
}

UTexture2D* UDigumInventorySlot::GetItemTexture() const
{
	if(ItemObject.IsValid())
	{
		return ItemObject->GetItemTexture();
	}
	
	return nullptr;
}
