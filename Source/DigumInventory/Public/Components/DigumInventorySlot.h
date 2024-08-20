// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Properties/DigumItem.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "UObject/Object.h"
#include "DigumInventorySlot.generated.h"

class ADigumItemActor;
class UDigumInventoryComponent;

UCLASS()
class DIGUMINVENTORY_API UDigumInventorySlot : public UObject
{
	GENERATED_BODY()
	
	TWeakObjectPtr<UDigumInventoryComponent> OwningInventory;

	UPROPERTY()
	TObjectPtr<UDigumItem> ItemObject;

	UPROPERTY()
	TArray<FString> UIDArray;
	
public:
	
	UDigumInventorySlot();

protected:
	DECLARE_MULTICAST_DELEGATE(FOnInventorySlotContentChanged);

	FOnInventorySlotContentChanged OnInventorySlotContentChanged;
public:
	bool bEmpty = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	FDigumItemProperties ItemProperties;

	UPROPERTY()
	int32 InventoryIndex = INDEX_NONE;
	
	bool HasValidItem() const { return ItemProperties.IsValid(); }
	FName GetItemID() const { return ItemProperties.ItemID; }
	int32 GetAmount() const { return ItemProperties.ItemAmount; }
	int32 GetInventoryIndex() const { return InventoryIndex; }
	int32 GetStackSize() const;
	int32 GetItemAmount() const;
	void SetAmount(const int32& InAmount) { ItemProperties.ItemAmount = InAmount; }
	
	// returns stack excess
	int32 IncrementAmount(const int32& InAmount);
	// returns remaining amount
	void DecrementAmount(const int32& InAmount);
	UDigumItem* GetItemObject() const { return ItemObject.Get(); }

	
	void SetItemProperties(const FDigumItemProperties& InItemProperties);
	void SetItemObject(UDigumItem* InItemObject);
	void Clear();
	void SwapContent(UDigumInventorySlot* InOtherSlot);
	
	UTexture2D* GetItemTexture() const;
	UMaterialInterface* GetDisplayMaterial();
	TSubclassOf<ADigumItemActor> GetItemActorClass() const;

	FOnInventorySlotContentChanged& GetOnInventorySlotContentChanged() { return OnInventorySlotContentChanged; }
	FDigumItemProperties GetItemProperties() const;
};