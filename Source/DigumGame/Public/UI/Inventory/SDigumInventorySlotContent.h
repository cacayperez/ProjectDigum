// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"

class UDigumInventorySlot;
/**
 * 
 */
class DIGUMGAME_API SDigumInventorySlotContent : public SDigumWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumInventorySlotContent)
	: _HeightOverride(100.0f)
	, _WidthOverride(100.0f)
	{}
	SLATE_ATTRIBUTE(UDigumInventorySlot*, InventorySlot)
	SLATE_ATTRIBUTE(float, HeightOverride)
	SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
	TWeakObjectPtr<UDigumInventorySlot> InventorySlot;
};
