// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumItemAffixAsset.generated.h"

UENUM()
enum class EDigumItemAffixType : uint8
{
	Prefix,
	Suffix
};




/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemAffixAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	EDigumItemAffixType AffixType;
};
