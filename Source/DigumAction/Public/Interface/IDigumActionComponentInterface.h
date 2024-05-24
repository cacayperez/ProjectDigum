// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/DigumActionComponent.h"
#include "IDigumActionComponentInterface.generated.h"

// class UDigumActionComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDigumActionComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIGUMACTION_API IIDigumActionComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UDigumActionComponent* GetActionComponent() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Digum Action Component", meta = (DisplayName = "Get Action Component"))
	UDigumActionComponent* GetActionComponentBP() const;
	
};
