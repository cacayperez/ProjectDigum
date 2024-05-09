// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"

class SDigumInventoryWindow;
class UDigumInventorySlot;
/**
 * 
 */
class DIGUMGAME_API SDigumInventorySlot : public SDigumWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumInventorySlot)
	: _HeightOverride(100.0f)
	, _WidthOverride(100.0f)
	{}
	SLATE_ATTRIBUTE(float, HeightOverride)
	SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_ATTRIBUTE(UDigumInventorySlot*, InventorySlot)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
	
protected:
	TWeakObjectPtr<UDigumInventorySlot> InventorySlot;
	TSharedPtr<SDigumInventoryWindow> InventoryWindow;

	virtual void OnReceiveDropPayload(UObject* InPayload) override;
public:
	void SetInventoryWindow(const TSharedPtr<SDigumInventoryWindow>& Window);
	
};
