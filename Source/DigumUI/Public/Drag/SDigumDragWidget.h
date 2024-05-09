// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"



/**
 * 
 */
class DIGUMUI_API SDigumDragWidget : public SDigumWidget
{
public:
	TWeakObjectPtr<UObject> Payload;
	UObject* GetDragPayload() const;
	virtual void OnConstruct() override;
	virtual void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void SetDragPayload(UObject* InPayload);

};
