// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItemPropertyBuilder.generated.h"

struct FDigumItemProperties;
class UDigumItem;
/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemPropertyBuilder : public UObject
{
	GENERATED_BODY()
public:
	virtual bool BuildItemProperties(const FDigumItemProperties& InItemProperties, UDigumItem*& OutBuiltItem);
};
