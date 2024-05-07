// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "UObject/Object.h"
#include "DigumInventorySlot.generated.h"

class UDigumInventoryComponent;

UCLASS()
class DIGUMINVENTORY_API UDigumInventorySlot : public UObject
{
	GENERATED_BODY()
	
	TWeakObjectPtr<UDigumInventoryComponent> OwningInventory;
	
public:
	bool bEmpty = true;
	
	UDigumInventorySlot();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	FDigumInventoryItemProperties ItemProperties;

	UPROPERTY()
	int32 InventoryIndex = INDEX_NONE;
	
	FName GetItemID() const { return ItemProperties.ItemID; }
	int32 GetAmount() const { return ItemProperties.ItemAmount; }
	void SetAmount(const int32 InAmount) { ItemProperties.ItemAmount = InAmount; }
	void SetItemProperties(const FDigumInventoryItemProperties& InItemProperties);
	void ClearItemProperties();
	bool IsValid() const { return ItemProperties.IsValid(); }

};