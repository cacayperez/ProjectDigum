// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
#include "UObject/Object.h"
#include "DigumWorldEditorSwatch.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditorSwatch : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName SwatchName;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDigumWorldSwatch> SoftSwatchAsset;

	bool IsValidSwatch() const
	{
		return SoftSwatchAsset.IsValid();
	}
};
