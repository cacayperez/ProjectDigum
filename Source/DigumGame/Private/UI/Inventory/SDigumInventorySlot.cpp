// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventorySlot.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"
#include "UI/Inventory/SDigumInventorySlotContent.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventorySlot::Construct(const FArguments& InArgs)
{
	InventorySlot = InArgs._InventorySlot.Get();
	
	SDigumWidget::Construct(SDigumWidget::FArguments()
	.HeightOverride(InArgs._HeightOverride)
	.WidthOverride(InArgs._WidthOverride));

	if(InventorySlot.IsValid())
	{
		ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Background"))
			]
			+ SOverlay::Slot()
			[
				SNew(SDigumInventorySlotContent)
			]
		];
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
