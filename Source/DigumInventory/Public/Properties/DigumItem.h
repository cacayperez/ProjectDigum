// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	TWeakObjectPtr<UTexture2D> DisplayTexture = nullptr;

	UPROPERTY()
	FName ItemID;
	
	UPROPERTY()
	int32 StackSize;

	int32 GetStackSize() const { return StackSize; }
	UTexture2D* GetItemTexture() const { return DisplayTexture.Get(); }
};
