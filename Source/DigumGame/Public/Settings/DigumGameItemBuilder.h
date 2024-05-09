// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Properties/DigumItemPropertyBuilder.h"
#include "UObject/Object.h"
#include "DigumGameItemBuilder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DIGUMGAME_API UDigumGameItemBuilder : public UDigumItemPropertyBuilder
{
	GENERATED_BODY()
public:
	// virtual UDigumItem* BuildItemProperties(const FDigumItemProperties& InItemProperties) override;
	virtual bool BuildItemProperties(const FDigumItemProperties& InItemProperties, UDigumItem*& OutBuiltItem) override;
};

