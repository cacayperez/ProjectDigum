// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/DigumInventoryComponent.h"

#include "Asset/DigumItem.h"
#include "Asset/DigumItemTable.h"
#include "Components/DigumInventorySlot.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "Properties/DigumItemPropertyBuilder.h"
#include "Settings/DigumInventorySettings.h"

DEFINE_LOG_CATEGORY(LogDigumInventory);
#define INVALID_SLOT_INDEX -1

UDigumInventoryComponent::UDigumInventoryComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	PrimaryComponentTick.bCanEverTick = false;
}

void UDigumInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void UDigumInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	

	// Initialize inventory array
	// Initialize inventory array
	const int32 Size = InitProperties.InventorySize;
	//InventoryItems.Init({}, Size);

	for(int32 i = 0; i < Size; i++)
	{
		UDigumInventorySlot* Slot = NewObject<UDigumInventorySlot>(this);
		Slot->InventoryIndex = i;
		InventoryItems.Add(Slot);
	}

	for(const auto Property : InitProperties.DefaultItems)
	{
		int32 Amount;
		if(AddItem_Internal(Property, Amount))
		{
			UE_LOG(LogDigumInventory, Warning, TEXT("Item %s added to inventory"), *Property.ItemID.ToString());
		}
		else
		{
			UE_LOG(LogDigumInventory, Error, TEXT("Item %s could not be added to inventory"), *Property.ItemID.ToString());
		
		}
	}
}

void UDigumInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UDigumInventoryComponent::BuildItemProperties(const FDigumItemProperties& InItemProperties,
	UDigumItem*& OutBuiltItem)
{
	// Override this function in a child class
	return false;
}

UDigumInventorySlot* UDigumInventoryComponent::GetItemSlot(const int32 InIndex) const
{
	if(InventoryItems.IsValidIndex(InIndex))
	{
		return InventoryItems[InIndex];
	}
	
	return nullptr;
}


bool UDigumInventoryComponent::FindItemsWithItemID(const FName& InItemID, TArray<UDigumInventorySlot*>& OutResult)
{
	bool bResult = false;
	TArray<UDigumInventorySlot*> ResultArray;
	for(auto Item : InventoryItems)
	{
		if(Item != nullptr)
		{
			if(Item->GetItemID() == InItemID)
			{
				ResultArray.Add(Item);
				bResult = true;
			}
		}

	}

	OutResult = ResultArray;
	return bResult;
}

int32 UDigumInventoryComponent::FindEmptySlot() const
{
	for(auto Slot : InventoryItems)
	{
		if(!Slot->IsValid()) return Slot->InventoryIndex;
	}
	return INVALID_SLOT_INDEX;
}

bool UDigumInventoryComponent::AddItem_Internal(const FDigumInventoryItemProperties& InItemProperties, int32& OutExcessAmount)
{
	
	UDigumItem* BuiltItem;
	BuildItemProperties(InItemProperties, BuiltItem);
	if(BuiltItem == nullptr)
	{
		UE_LOG(LogDigumInventory, Error, TEXT("Item with ID %s could not be built"), *InItemProperties.ItemID.ToString());
		return false;
	}
	
	const int32 StackSize = BuiltItem->GetStackSize();
	int32 RemainingAmount = BuiltItem->GetItemAmount();
	
	if(StackSize <= 0)
	{
		// Log assertion error
		UE_LOG(LogDigumInventory, Error, TEXT("Item with ID %s has an invalid stack size of %d"), *InItemProperties.ItemID.ToString(), StackSize);
		return false;
	}
	
	// TODO add stacking items
	// Find all items with the same ID
	TArray<UDigumInventorySlot*> FoundItems;
	
	if(FindItemsWithItemID(InItemProperties.ItemID, FoundItems))
	{
		// Loop through the items and add the current Amount;
		for(auto Item : FoundItems)
		{
			const int32 ItemAmount = Item->GetAmount();
			const int32 RemainingStackSpace = StackSize - ItemAmount;
			
			if(RemainingStackSpace > 0)
			{
				const int32 AmountToAdd = FMath::Min(RemainingAmount, RemainingStackSpace);
				const int32 NewAmount = ItemAmount + AmountToAdd;
				Item->SetAmount(NewAmount);

				RemainingAmount -= AmountToAdd;

				if(RemainingAmount <= 0)
				{
					return true;
				}
			}
		}
	}
	
	int32 ExcessAmount = RemainingAmount;
	// if there is still an amount left, find an empty slot and add the item
	while (ExcessAmount > 0)
	{
		UDigumInventorySlot* EmptySlot = GetItemSlot(FindEmptySlot());

		if(EmptySlot != nullptr)
		{
			FDigumInventoryItemProperties NewItemProperties = InItemProperties;
			const int32 AmountToAdd = FMath::Min(RemainingAmount, StackSize);
			NewItemProperties.ItemAmount = AmountToAdd;

			RemainingAmount -= AmountToAdd;
			ExcessAmount = RemainingAmount;

			EmptySlot->SetItemProperties(NewItemProperties);
		}
		else
		{
			// force exit loop
			ExcessAmount = 0;
			UE_LOG(LogDigumInventory, Warning, TEXT("No empty slots found"));
		}
	}

	OutExcessAmount = RemainingAmount;

#if WITH_EDITOR // Debug items
	for(auto Slot : InventoryItems)
	{
		UE_LOG(LogDigumInventory, Warning, TEXT("Slot %d: %s"), Slot->InventoryIndex, *Slot->GetItemID().ToString());
	}
#endif
	
	if(OutExcessAmount <= 0)
	{
		return true;
	}

	return false;
}


TArray<UDigumInventorySlot*> UDigumInventoryComponent::GetInventoryItems_Internal() const
{
	return InventoryItems;
}

TArray<UDigumInventorySlot*> UDigumInventoryComponent::GetInventoryItems() const
{
	return GetInventoryItems_Internal();
}

bool UDigumInventoryComponent::RemoveItemFromSlot(const int32 InSlotIndex, const int32 InAmount)
{
	UDigumInventorySlot* Slot = GetItemSlot(InSlotIndex);
	if(Slot != nullptr)
	{
		Slot->ClearItemProperties();
		return true;
	}
	
	return false;
}


