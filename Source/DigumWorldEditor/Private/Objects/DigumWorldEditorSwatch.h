// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldEditorSwatch.generated.h"

class UDigumWorldSwatchAsset;
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
	TSoftObjectPtr<UDigumWorldSwatchAsset> SoftSwatchAsset;

	bool IsValidSwatch() const;
	
};
