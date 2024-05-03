// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumItemProperties.h"
#include "UObject/Object.h"
#include "DigumInventoryItemProperties.generated.h"

struct FDigumItemProperties;
/**
 * 
 */
USTRUCT(BlueprintType)
struct DIGUMINVENTORY_API FDigumInventoryItemProperties : public FDigumItemProperties
{
	GENERATED_BODY()
public:
	FDigumInventoryItemProperties()
	{
	}
	
};
