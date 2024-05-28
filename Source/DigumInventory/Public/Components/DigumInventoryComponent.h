// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumInventorySlot.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "DigumInventoryComponent.generated.h"


class ADigumItemActor;
class UDigumItem;
DECLARE_LOG_CATEGORY_EXTERN(LogDigumInventory, Log, All);

USTRUCT(BlueprintType)
struct FDigumInventoryInitProperties
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 InventorySize = 10;

	UPROPERTY(EditAnywhere)
	TArray<FDigumInventoryItemProperties> DefaultItems;
};




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMINVENTORY_API UDigumInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	TArray<UDigumInventorySlot*> InventoryItems;

public:
	// Sets default values for this component's properties
	UDigumInventoryComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:

	DECLARE_MULTICAST_DELEGATE(FOnInventoryContentChanged);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemDropped, const FDigumItemProperties);

	FOnInventoryContentChanged OnInventoryContentChanged;
	FOnItemDropped OnItemDropped;
	
	virtual bool BuildItemProperties(const FDigumItemProperties& InItemProperties, UDigumItem*& OutBuiltItem);
	virtual void OnItemDrop(const FDigumItemProperties& InItemProperties);
	// virtual bool BuildItemProperties(const FDigumItemProperties& InItemProperties, UDigumItem*& OutBuiltItem);

	
private:
	UDigumInventorySlot* GetItemSlot(const int32 InIndex) const;
	bool FindItemsWithItemID(const FName& InItemID, TArray<UDigumInventorySlot*>& OutResult);
	int32 FindEmptySlot() const;
	
	bool AddItem_Internal(const FDigumInventoryItemProperties& InItemProperties, int32& OutExcessAmount);
	TArray<UDigumInventorySlot*> GetInventoryItems_Internal() const;
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	FDigumInventoryInitProperties InitProperties;

	UFUNCTION(BlueprintCallable, Category = "Digum Inventory", meta=(DisplayName="Get Inventory Items"))
	TArray<UDigumInventorySlot*> GetInventoryItems() const;

	void OnInventorySlotContentChanged();
	bool RemoveItemFromSlot(const int32 InSlotIndex, const int32 InAmount = 1);
	TSubclassOf<ADigumItemActor> GetItemActorClass(const int32 InSlotIndex) const;
	
	template<typename T>
	T* GetItem(const int32 InSlotIndex) const;

	FOnInventoryContentChanged& GetOnInventoryContentChangedDelegate() { return OnInventoryContentChanged; }
	FOnItemDropped& GetOnItemDroppedDelegate() { return OnItemDropped; }
	
	void TryDropItem(const int32& InSlotIndex);
};

template <typename T>
T* UDigumInventoryComponent::GetItem(const int32 InSlotIndex) const
{
	static_assert(std::is_base_of<UDigumItem, T>::value, "T must be a derived class of UDigumItem");
	UDigumInventorySlot* Slot = GetItemSlot(InSlotIndex);
	if(Slot != nullptr)
	{
		return static_cast<T*>(Slot->GetItemObject());
	}
	
	return nullptr;
}
