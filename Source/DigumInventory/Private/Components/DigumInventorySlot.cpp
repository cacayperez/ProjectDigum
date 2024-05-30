// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumInventorySlot.h"

#include "Asset/DigumItemAsset.h"
#include "Asset/DigumItemTable.h"
#include "Components/DigumInventoryComponent.h"
UDigumInventorySlot::UDigumInventorySlot()
{
}

int32 UDigumInventorySlot::GetStackSize() const
{
	return ItemObject ? ItemObject->GetStackSize() : 0; 
}

int32 UDigumInventorySlot::GetItemAmount() const
{
	return ItemProperties.GetAmount();
}

int32 UDigumInventorySlot::IncrementAmount(const int32& InAmount)
{
	const int32 StackSize = GetStackSize();
	const int32 CurrentAmount = GetItemAmount();
	const int32 NewAmount = FMath::Clamp(CurrentAmount + InAmount, 0, StackSize);
	const int32 Excess = FMath::Max(0, CurrentAmount + InAmount - StackSize);

	SetAmount(NewAmount);

	if(CurrentAmount != NewAmount)
	{
		OnInventorySlotContentChanged.Broadcast();
	}
	
	return Excess;
}

void UDigumInventorySlot::DecrementAmount(const int32& InAmount)
{
	const int32 CurrentAmount = GetItemAmount();
	const int32 NewAmount = FMath::Clamp(CurrentAmount - InAmount, 0, GetStackSize());

	SetAmount(NewAmount);

	if(NewAmount == 0)
	{
		Clear();
	}

	if(CurrentAmount != NewAmount)
	{
		OnInventorySlotContentChanged.Broadcast();
	}
}

void UDigumInventorySlot::SetItemProperties(const FDigumItemProperties& InItemProperties)
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
	
	const FDigumItemProperties ThisProperties = ItemProperties;
	const FDigumItemProperties OtherProperties = InOtherSlot->ItemProperties;
	UDigumItem* ThisItemObject = ItemObject.Get();
	UDigumItem* OtherItemObject = InOtherSlot->GetItemObject();
	
	InOtherSlot->SetItemProperties(ThisProperties);
	InOtherSlot->SetItemObject(ThisItemObject);
	
	SetItemProperties(OtherProperties);
	SetItemObject(OtherItemObject);
	OnInventorySlotContentChanged.Broadcast();
}

UTexture2D* UDigumInventorySlot::GetItemTexture() const
{
	if(ItemObject)
	{
		return ItemObject->GetItemTexture();
	}
	UE_LOG(LogTemp, Warning, TEXT("UDigumInventorySlot::GetItemTexture - ItemObject is null"));
	return nullptr;
}

UMaterialInterface* UDigumInventorySlot::GetDisplayMaterial()
{
	if(ItemObject)
	{
		return ItemObject->GetDisplayMaterial();
	}
	UE_LOG(LogTemp, Warning, TEXT("UDigumInventorySlot::GetDisplayMaterial - ItemObject is null"));
	return nullptr;
}

TSubclassOf<ADigumItemActor> UDigumInventorySlot::GetItemActorClass() const
{
	if(ItemObject)
	{
		return ItemObject->GetItemActorClass();
	}
	UE_LOG(LogTemp, Warning, TEXT("UDigumInventorySlot::GetDisplayMaterial - ItemObject is null"));
	return nullptr;
}

FDigumItemProperties UDigumInventorySlot::GetItemProperties() const
{
	return ItemProperties;
}
