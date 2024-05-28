// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/DigumInventoryWidget.h"
#include "DigumActionBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumActionBarWidget : public UDigumInventoryWidget
{
	GENERATED_BODY()
public:
	virtual void OnCreateWidget() override;
	
};
