// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWorldEditorSelector.h"
#include "UObject/Object.h"
#include "DigumWorldEditorSelector_Single.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditorSelector_Single : public UDigumWorldEditorSelector
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<SWidget> CreateSelectorButtonWidget() override;
	virtual void BeginSelection() override;
	virtual void EndSelection() override;
	virtual void AddSelection(FDigumWorldAssetCoordinate Coordinate) override;
};
