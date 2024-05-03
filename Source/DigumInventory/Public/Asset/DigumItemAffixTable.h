// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumItemAffixTable.generated.h"

class UDigumItemAffixAsset;

USTRUCT()
struct FDigumItemAffixTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	FName AffixID;

	TSoftObjectPtr<UDigumItemAffixAsset> AffixAsset;
};

/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemAffixTable : public UObject
{
	GENERATED_BODY()
};
