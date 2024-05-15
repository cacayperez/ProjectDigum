// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "DigumPickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDigumPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMINVENTORY_API IDigumPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPickup(AActor* InPickupInstigator) = 0;
	/*virtual int32 GetAmount() const = 0;
	virtual FName GetItemID() const = 0;
	virtual FName GetItemSuffix() const = 0;
	virtual FName GetItemPrefix() const = 0;*/

};
