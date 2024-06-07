// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDigumWorldPawnInterface.generated.h"

class UDigumWorldPositioningComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDigumWorldPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMWORLD_API IIDigumWorldPawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UDigumWorldPositioningComponent* GetPositioningComponent() const = 0;
};
