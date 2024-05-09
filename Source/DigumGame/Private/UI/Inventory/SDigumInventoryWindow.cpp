// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventoryWindow.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventoryComponent.h"
#include "Components/DigumInventorySlot.h"
#include "Core/SDigumWidgetStack.h"
#include "Drag/SDigumDragWidget.h"
#include "UI/Inventory/SDigumInventorySlot.h"

#include "Templates/SharedPointer.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SDigumInventoryWindow::~SDigumInventoryWindow()
{
	InventoryItemSlotsWidgets.Empty();
}

void SDigumInventoryWindow::Construct(const FArguments& InArgs)
{
	WeakInventoryComponent = InArgs._InventoryComponent.Get();
	SDigumWindow::Construct(SDigumWindow::FArguments()
		.ParentContainer(InArgs._ParentContainer)
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride)
	);

	// Set Window Width and Height based on GridWidth and GridHeight
	float Width = GridWidth * 100;
	float Height = GridHeight * 100;

	HeightOverrideAttribute = Height;
	WidthOverrideAttribute = Width;
}

void SDigumInventoryWindow::OnConstruct()
{
	SDigumWindow::OnConstruct();
	
	DrawWindow();
}

void SDigumInventoryWindow::OnReceiveDropPayload(UObject* InPayload)
{
	FVector2D CursorPosition = FSlateApplication::Get().GetCursorPos();
	for(auto ItemSlot : InventoryItemSlotsWidgets)
	{
		if(ItemSlot->GetWidgetGeometry().IsUnderLocation(CursorPosition))
		{
			ItemSlot->ReceiveDropPayload(InPayload);
			return;
		}
	}
	
}
void SDigumInventoryWindow::BeginDragItem(UDigumInventorySlot* ItemSlot)
{
	TSharedPtr<SDigumDragWidget> DragWidget = SNew(SDigumDragWidget);

	DragWidget->SetDragPayload(ItemSlot);
	if(DragWidget.IsValid() && _ParentContainer.IsValid())
	{
		_ParentContainer->AddDraggableItemToStack(DragWidget);
	}
}

void SDigumInventoryWindow::StopDragItem()
{
	if(_ParentContainer.IsValid())
	{
		UObject* OutPayload;
		// True if dropped on another widget
		bool bWidgetHandled = _ParentContainer->RemoveDraggedItemFromStack(OutPayload);

		// Assume that it is dropped in the world
		if(bWidgetHandled == false && OutPayload != nullptr)
		{
			UDigumInventoryComponent* InventoryComponent = WeakInventoryComponent.Get();
			const UDigumInventorySlot* ItemSlot = Cast<UDigumInventorySlot>(OutPayload);
			
			if(ItemSlot == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("ItemSlot is null"));
			}
			
			if(InventoryComponent && ItemSlot)
			{
				int32 Index = ItemSlot->InventoryIndex;
				InventoryComponent->RemoveItemFromSlot(ItemSlot->InventoryIndex);
			
			}
		}
	}

	Refresh();
}


TSharedPtr<SWidget> SDigumInventoryWindow::OnCreateContent()
{
	TSharedPtr<SGridPanel> GridPanel = SNew(SGridPanel);
	InventoryItemSlotsWidgets.Empty();

	UDigumInventoryComponent* InventoryComponent = WeakInventoryComponent.Get();
	if(InventoryComponent != nullptr)
	{
		TArray<UDigumInventorySlot*> InventoryItems = InventoryComponent->GetInventoryItems();
		for(int32 i = 0; i < InventoryItems.Num(); i++)
		{
			int32 Row = i / GridWidth;
			int32 Column = i % GridWidth;
			TSharedPtr<SDigumInventorySlot> ItemWidget = CreateWidgetItem(InventoryItems[i]);
			ItemWidget->SetInventoryWindow(SharedThis(this));
			InventoryItemSlotsWidgets.Add(ItemWidget.ToSharedRef());
			
			if(ItemWidget.IsValid())
			{
				GridPanel->AddSlot(Column, Row)
				[
					ItemWidget.ToSharedRef()
				];
			}
		}
	}
	return GridPanel;
}

TSharedPtr<SDigumInventorySlot> SDigumInventoryWindow::CreateWidgetItem(UDigumInventorySlot* Item) const
{
	if(Item != nullptr)
	{
		
		TSharedPtr<SDigumInventorySlot> Slot
			= SNew(SDigumInventorySlot)
			.InventorySlot(Item)
			.HeightOverride(100)
			.WidthOverride(100);

		
		return Slot;
	}
	return nullptr;
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION

