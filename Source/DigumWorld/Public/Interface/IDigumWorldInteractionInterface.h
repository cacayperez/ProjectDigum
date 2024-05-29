// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDigumWorldInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDigumWorldInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMWORLD_API IIDigumWorldInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category = "DigumWorld|Interaction")
	void OnInteract(const AActor* InInstigator, const FDigumWorldRequestParams& InParams);
};
