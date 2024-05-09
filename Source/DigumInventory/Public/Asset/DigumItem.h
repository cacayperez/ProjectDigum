// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItem.generated.h"






/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTexture2D* DisplayTexture = nullptr;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 ItemAmount;

	UPROPERTY()
	int32 StackSize;

	int32 GetStackSize() const { return StackSize; }
	int32 GetItemAmount() const { return ItemAmount; }
};
