// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWidgetStyle.h"
#include "DigumWindowStyle.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMUI_API UDigumWindowStyle : public UDigumWidgetStyle
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Settings")
	USlateWidgetStyleAsset* Style;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush")
	USlateBrushAsset* HeaderImage;
};
