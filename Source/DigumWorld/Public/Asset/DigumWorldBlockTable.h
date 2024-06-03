// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldBlockTable.generated.h"

class UDigumWorldSwatchAsset;
USTRUCT()
struct FDigumWorldBlockTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName BlockID = NAME_None;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;
};
/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldBlockTable : public UObject
{
	GENERATED_BODY()
};
