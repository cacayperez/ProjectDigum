// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Properties/DigumInventoryItemProperties.h"
#include "DigumInventoryComponent.generated.h"


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
	
};
