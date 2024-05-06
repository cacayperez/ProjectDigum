// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "List/SDigumListSlotDraggable.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumInventoryItemListSlot : public SDigumListSlotDraggable
{
public:
	virtual void OnDrawWidget() override;

private:
	TSharedPtr<SWidget> CreateWidgetSlot();
};
