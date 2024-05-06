// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "List/SDigumList.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumInventoryItemList : public SDigumList
{
public:
	virtual TSharedPtr<SWidget> OnCreateItem(UObject* Item) const override;
};
