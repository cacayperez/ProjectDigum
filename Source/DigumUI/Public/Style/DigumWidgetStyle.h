// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SlateBrushAsset.h"
#include "UObject/Object.h"
#include "DigumWidgetStyle.generated.h"

class USlateBrushAsset;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class DIGUMUI_API UDigumWidgetStyle : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush")
	USlateBrushAsset* Image;
	
};
