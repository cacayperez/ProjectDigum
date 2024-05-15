// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/ActionBar/SDigumActionBar.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDigumActionBar::OnConstruct()
{
	SDigumWidget::OnConstruct();
}

void SDigumActionBar::OnReceiveDropPayload(UObject* InPayload)
{
	SDigumWidget::OnReceiveDropPayload(InPayload);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
