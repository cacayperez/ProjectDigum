// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumItemAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	int32 StackSize;
};
