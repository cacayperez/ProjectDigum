// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DigumExternalStorageInterface.generated.h"

class UDigumInventoryComponent;
// This class does not need to be modified.
UINTERFACE()
class UDigumExternalStorageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMINVENTORY_API IDigumExternalStorageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UDigumInventoryComponent* GetInventoryComponent() const = 0;
};
