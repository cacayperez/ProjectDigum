// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "List/SDigumListSlot.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumListSlot::Construct(const FArguments& InArgs)
{
	ItemAttribute = InArgs._Item;
	HeightAttribute = InArgs._Height;
	WidthAttribute = InArgs._Width;
	
	DrawWidget();
}

FVector2D SDigumListSlot::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(WidthAttribute.Get(), HeightAttribute.Get()) * LayoutScaleMultiplier;
}

void SDigumListSlot::DrawWidget()
{
	OnDrawWidget();
}

void SDigumListSlot::OnDrawWidget()
{
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
