// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/DigumInventoryWidget.h"

#include "Character/Miner/Components/DigumGameInventoryComponent.h"
#include "UI/Inventory/SDigumInventoryWindow.h"


UDigumInventoryWidget::~UDigumInventoryWidget()
{
	InventorySlate.Reset();
}

void UDigumInventoryWidget::OnCreateWidget()
{
	const float Width = ConstructParameters.Size.X;
	const float Height = ConstructParameters.Size.Y;
	
	InventorySlate =
		SNew(SDigumInventoryWindow)
		.BackgroundMaterial(BackgroundMaterial)
		.InventorySlotMaterial(InventorySlotMaterial)
		.WindowStyle(&WindowStyle)
		.HeightOverride(Height)
		.WidthOverride(Width) ;

}

TSharedPtr<SDigumWidget> UDigumInventoryWidget::GetWidget() const
{
	return InventorySlate;
}

void UDigumInventoryWidget::OnUpdateContent()
{
	if(InventorySlate)
	{
		InventorySlate->UpdateInventoryGridPanel();
	}
}

void UDigumInventoryWidget::SetInventoryComponent(UDigumGameInventoryComponent* InventoryComponent)
{
	if(InventorySlate && InventoryComponent)
	{
		InventoryComponent->GetOnInventoryContentChangedDelegate().AddUObject(this, &UDigumInventoryWidget::OnUpdateContent);
		InventorySlate->SetInventoryComponent(InventoryComponent);
		
	}
}
