// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventoryWindow.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventoryComponent.h"
#include "Components/DigumInventorySlot.h"
#include "UI/Inventory/SDigumInventorySlot.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventoryWindow::Construct(const FArguments& InArgs)
{
	WeakInventoryComponent = InArgs._InventoryComponent.Get();
	SDigumWindow::Construct(SDigumWindow::FArguments()
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride)
	);

	// Set Window Width and Height based on GridWidth and GridHeight
	float Width = GridWidth * 100;
	float Height = GridHeight * 100;

	HeightOverrideAttribute = Height;
	WidthOverrideAttribute = Width;
}

TSharedPtr<SWidget> SDigumInventoryWindow::OnCreateContent()
{
	_Container = SNew(SGridPanel);

	UDigumInventoryComponent* InventoryComponent = WeakInventoryComponent.Get();
	if(InventoryComponent != nullptr)
	{
		TArray<UDigumInventorySlot*> InventoryItems = InventoryComponent->GetInventoryItems();
		for(int32 i = 0; i < InventoryItems.Num(); i++)
		{
			int32 Row = i / GridWidth;
			int32 Column = i % GridWidth;
			TSharedPtr<SWidget> ItemWidget = CreateWidgetItem(InventoryItems[i]);
			if(ItemWidget.IsValid())
			{
				_Container->AddSlot(Column, Row)
				[
					ItemWidget.ToSharedRef()
				];
			}
		}
	}
	
	return _Container;
}

TSharedPtr<SWidget> SDigumInventoryWindow::CreateWidgetItem(UDigumInventorySlot* Item) const
{
	if(Item != nullptr)
	{
		return SNew(SDigumInventorySlot)
			.InventorySlot(Item)
			.HeightOverride(100)
			.WidthOverride(100);
	}
	return nullptr;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

