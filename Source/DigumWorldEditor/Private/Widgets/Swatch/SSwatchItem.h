// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/Base/SWidgetBase.h"

class UDigumWorldSwatchAsset;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SSwatchItem : public SWidgetBase
{
public:
	SLATE_BEGIN_ARGS(SSwatchItem) {}
	SLATE_ARGUMENT(bool, bIsActive);
	SLATE_ATTRIBUTE(UDigumWorldSwatchAsset*, SwatchAsset);
	SLATE_END_ARGS()

	void OnSelectSwatch();
	void OnDeselectSwatch();
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;

protected:
	bool bIsActiveSwatch = false;
	TAttribute<UDigumWorldSwatchAsset*> AssetAttribute;
	// TAttribute<FDigumWorldSwatchPaletteItem> SwatchItemAttribute;	
};
