// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindowHeader.h"
#include "Rendering/DrawElements.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumWindowHeader::Construct(const FArguments& InArgs)
{
	SDigumWidget::Construct(
		SDigumWidget::FArguments()
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride));
	
	
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromString("Header"))
	];
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
