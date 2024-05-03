// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/DigumInventoryComponent.h"
#include "Asset/DigumItemTable.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "Settings/DigumInventorySettings.h"

DEFINE_LOG_CATEGORY(LogDigumInventory);
#define INVALID_SLOT_INDEX -1

UDigumInventoryComponent::UDigumInventoryComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDigumInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Initialize inventory array
	InventoryItems.Init({}, InitProperties.InventorySize);

	for(int32 i = 0; i < InventoryItems.Num(); i++)
	{
		UDigumInventoryItemSlot* Slot = NewObject<UDigumInventoryItemSlot>(this);
		Slot->InventoryIndex = i;
		InventoryItems[i] = Slot;
	}

	
}

void UDigumInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize inventory array

	
}

void UDigumInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UDigumInventoryItemSlot* UDigumInventoryComponent::GetItemSlot(const int32 InIndex) const
{
	if(InventoryItems.IsValidIndex(InIndex))
	{
		return InventoryItems[InIndex];
	}
	
	return nullptr;
}


bool UDigumInventoryComponent::FindItemsWithItemID(const FName& InItemID, TArray<UDigumInventoryItemSlot*>& OutResult)
{
	bool bResult = false;
	TArray<UDigumInventoryItemSlot*> ResultArray;
	for(auto Item : InventoryItems)
	{
		if(Item->GetItemID() == InItemID)
		{
			ResultArray.Add(Item);
			bResult = true;
		}
	}

	OutResult = ResultArray;
	return bResult;
}

int32 UDigumInventoryComponent::FindEmptySlot() const
{
	return INVALID_SLOT_INDEX;
}

bool UDigumInventoryComponent::AddItem_Internal(const FDigumInventoryItemProperties& InItemProperties, int32& OutExcessAmount)
{
	int32 RemainingAmount = InItemProperties.GetAmount();
	const UDigumInventorySettings* Settings = GetDefault<UDigumInventorySettings>();

	check(Settings);
	
	const int32 StackSize = UDigumItemTable::GetDigumItemStackSize(InItemProperties.ItemID, Settings->ItemTable.LoadSynchronous());

	if(StackSize <= 0)
	{
		// Log assertion error
		UE_LOG(LogDigumInventory, Error, TEXT("Item with ID %s has an invalid stack size of %d"), *InItemProperties.ItemID.ToString(), StackSize);
		return false;
	}
	
	// TODO add stacking items
	// Find all items with the same ID
	TArray<UDigumInventoryItemSlot*> FoundItems;
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
		UDigumInventoryItemSlot* EmptySlot = GetItemSlot(FindEmptySlot());

		if(EmptySlot != nullptr)
		{
			FDigumInventoryItemProperties NewItemProperties = InItemProperties;
			const int32 AmountToAdd = FMath::Min(RemainingAmount, StackSize);
			NewItemProperties.ItemAmount = AmountToAdd;

			RemainingAmount -= AmountToAdd;
			ExcessAmount = RemainingAmount;
		}
		else
		{
			// force exit loop
			ExcessAmount = 0;
		}
	}

	OutExcessAmount = RemainingAmount;
	
	return false;
}

TArray<UDigumInventoryItemSlot*> UDigumInventoryComponent::GetInventoryItems_Internal() const
{
	return InventoryItems;
}

TArray<UDigumInventoryItemSlot*> UDigumInventoryComponent::GetInventoryItems() const
{
	return GetInventoryItems_Internal();
}


