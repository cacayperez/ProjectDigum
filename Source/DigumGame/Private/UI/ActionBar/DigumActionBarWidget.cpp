// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/ActionBar/DigumActionBarWidget.h"

#include "UI/ActionBar/SDigumActionBarWindow.h"
#include "UI/Inventory/SDigumInventoryWindow.h"

void UDigumActionBarWidget::OnCreateWidget()
{
	const float Width = ConstructParameters.Size.X;
	const float Height = ConstructParameters.Size.Y;
	
	InventorySlate =
		SNew(SDigumActionBarWindow)
		.BackgroundMaterial(BackgroundMaterial)
		.InventorySlotMaterial(InventorySlotMaterial)
		.GridHeight(1)
		.WindowStyle(&WindowStyle)
		.HeightOverride(Height)
		.WidthOverride(Width) ;
}
