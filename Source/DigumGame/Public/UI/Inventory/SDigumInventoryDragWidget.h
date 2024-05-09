// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Drag/SDigumDragWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumInventoryDragWidget : public SDigumDragWidget
{
public:
	virtual void OnSetDragPayload(UObject* InPayload) override;
};
