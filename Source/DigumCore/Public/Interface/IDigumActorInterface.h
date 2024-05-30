// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDigumActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDigumActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMCORE_API IIDigumActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetForwardDirection() const = 0;
};
