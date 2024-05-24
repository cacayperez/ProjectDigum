// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumItemActor.h"
#include "UObject/Object.h"
#include "DigumItem.generated.h"

/**
 * For storing database information about an item
 * that are not stored in a save file
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UMaterialInterface> DisplayMaterial = nullptr;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> DisplayTexture = nullptr;

	UPROPERTY()
	TSubclassOf<ADigumItemActor> ItemActorClass = nullptr;

	UPROPERTY()
	FText ItemName;

	UPROPERTY()
	FText ItemDescription;
	
	UPROPERTY()
	FName ItemID;
	
	UPROPERTY()
	int32 StackSize;

	int32 GetStackSize() const { return StackSize; }
	UTexture2D* GetItemTexture() const { return DisplayTexture; }
	UMaterialInterface* GetItemMaterial() const { return DisplayMaterial; }
	UMaterialInterface* GetDisplayMaterial() const { return DisplayMaterial; }
	TSubclassOf<ADigumItemActor> GetItemActorClass() const { return ItemActorClass; }
};
