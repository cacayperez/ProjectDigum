// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventoryWindow.h"

#include "SlateOptMacros.h"
#include "Character/Miner/Components/DigumGameInventoryComponent.h"
#include "Components/DigumInventoryComponent.h"
#include "Components/DigumInventorySlot.h"
#include "Core/SDigumWidgetStack.h"
#include "Drag/SDigumDragWidget.h"
#include "UI/Inventory/SDigumInventorySlot.h"

#include "Templates/SharedPointer.h"
#include "UI/Inventory/SDigumInventoryDragWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SDigumInventoryWindow::~SDigumInventoryWindow()
{
	InventoryItemSlotsWidgets.Empty();
}

void SDigumInventoryWindow::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	WeakInventoryComponent = InArgs._InventoryComponent.Get();
	InventorySlotMaterialAttribute = InArgs._InventorySlotMaterial;
	GridHeightAttribute = InArgs._GridHeight;
	GridWidthAttribute = InArgs._GridWidth;
	SDigumWindow::Construct(SDigumWindow::FArguments()
		.WindowStyle(InArgs._WindowStyle)
		.BackgroundMaterial(InArgs._BackgroundMaterial)
		.ParentContainer(InArgs._ParentContainer)
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride)
	);
}

void SDigumInventoryWindow::OnConstruct()
{
	SDigumWindow::OnConstruct();
	DrawWindow();
	UpdateInventoryGridPanel();
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
	if(bIsTransactingItem) return;
	
	TSharedPtr<SDigumInventoryDragWidget> DragWidget = SNew(SDigumInventoryDragWidget);
	DragWidget->SetDragPayload(ItemSlot);
	
	if(_ParentContainer.IsValid() && ItemSlot->HasValidItem())
	{
		_ParentContainer->AddDraggableItemToStack(DragWidget);
		bIsTransactingItem = true;
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
			UDigumGameInventoryComponent* InventoryComponent = WeakInventoryComponent.Get();
			const UDigumInventorySlot* ItemSlot = Cast<UDigumInventorySlot>(OutPayload);
			
			if(ItemSlot == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("ItemSlot is null"));
			}
			
			if(InventoryComponent && ItemSlot)
			{
				const int32 Index = ItemSlot->InventoryIndex;
				// TO-DO Call DropItemInWorld
				InventoryComponent->TryDropItem(Index);
			}
		}
	}
	bIsTransactingItem = false;
	UpdateInventoryGridPanel();
}


TSharedPtr<SWidget> SDigumInventoryWindow::OnCreateContent()
{
	InventoryGridPanel = SNew(SGridPanel);
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.VAlign(VAlign_Center)
	[
		InventoryGridPanel.ToSharedRef()
	];
}

TSharedPtr<SDigumInventorySlot> SDigumInventoryWindow::CreateWidgetItem(UDigumInventorySlot* Item) const
{
	if(Item != nullptr)
	{
		TSharedPtr<SDigumInventorySlot> Slot
			= SNew(SDigumInventorySlot)
			.BackgroundMaterial(InventorySlotMaterialAttribute.Get())
			.InventorySlot(Item)
			.HeightOverride(120)
			.WidthOverride(120);

		
		return Slot;
	}
	return nullptr;
}

void SDigumInventoryWindow::UpdateInventoryGridPanel()
{
	InventoryGridPanel->ClearChildren();
	InventoryItemSlotsWidgets.Empty();

	UDigumInventoryComponent* InventoryComponent = WeakInventoryComponent.Get();
	if(InventoryComponent != nullptr)
	{
		TArray<UDigumInventorySlot*> InventoryItems = InventoryComponent->GetInventoryItems();
		const int32 Width = GridWidthAttribute.Get();
		const int32 Height = GridHeightAttribute.Get();
		int32 Index = 0;
		const int32 MaxRows = FMath::CeilToInt(static_cast<float>(InventoryItems.Num() / Width));
		for(int Row = 0; Row < Height; Row++)
		{
			if(Row >= MaxRows) break;
			
			for(int Column = 0; Column < Width; Column++)
			{
				TSharedPtr<SDigumInventorySlot> ItemWidget = CreateWidgetItem(InventoryItems[Index]);
				ItemWidget->SetInventoryWindow(SharedThis(this));
				InventoryItemSlotsWidgets.Add(ItemWidget.ToSharedRef());
				
				if(ItemWidget.IsValid())
				{
					InventoryGridPanel->AddSlot(Column, Row)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(3)
					[
						ItemWidget.ToSharedRef()
					];
				}
				Index++;
			}
		}
		
	}
}

void SDigumInventoryWindow::SetInventoryComponent(UDigumGameInventoryComponent* InInventoryComponent)
{
	if(InInventoryComponent == nullptr) return;
	
	WeakInventoryComponent = InInventoryComponent;
	UpdateInventoryGridPanel();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

