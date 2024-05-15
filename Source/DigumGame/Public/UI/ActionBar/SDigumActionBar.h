// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumActionBar : public SDigumWidget
{
public:
	virtual void OnConstruct() override;
	virtual void OnReceiveDropPayload(UObject* InPayload) override;
	
};
