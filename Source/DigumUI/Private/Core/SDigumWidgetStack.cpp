// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Core/SDigumWidgetStack.h"
#include "Core/SDigumWidget.h"
#include "Drag/SDigumDragWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SDigumWidgetStack::~SDigumWidgetStack()
{
	DraggedWidget = nullptr;
}

void SDigumWidgetStack::AddToStack_Internal(const TSharedPtr<SDigumWidget>& Item, const int32 ZOrder)
{
	const TSharedRef<SDigumWidget> Shared = Item.ToSharedRef();
	
	AddSlot(ZOrder)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		Shared
	];

	Shared->SetParentContainer(SharedThis(this));
	StackItems.Add(Shared);
}

void SDigumWidgetStack::RemoveFromStack_Internal(const TSharedPtr<SDigumWidget>& Item)
{
	int32 OutIndex;
	if(StackItems.Find(Item.ToSharedRef(), OutIndex))
	{
		StackItems.RemoveAt(OutIndex, 1, true);
		RemoveSlot(Item.ToSharedRef());
	}
}

void SDigumWidgetStack::ClearStack_Internal()
{
	for(auto Item : StackItems)
	{
		RemoveSlot(Item);
	}
	
	StackItems.Empty();
}

bool SDigumWidgetStack::DoesWidgetExist(const TSharedPtr<SDigumWidget>& Item) const
{
	return StackItems.Contains(Item.ToSharedRef());
}

void SDigumWidgetStack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SOverlay::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	

	/*for(const TSharedRef<SDigumWidget>& Item : StackItems)
	{
		if(Item->GetWidgetGeometry().IsUnderLocation(MousePosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Mouse is over widget %s"), *Item->GetTypeAsString());
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("NOT"));
	}*/
}


bool SDigumWidgetStack::DoesOverlapAnyChildren(const FVector2D& Position, TSharedPtr<SDigumWidget>& OutWidget) const
{
	for(const TSharedRef<SDigumWidget>& Item : StackItems)
	{
		if(Item->GetWidgetGeometry().IsUnderLocation(Position))
		{
			OutWidget = Item;
			return true;
		}
	}

	return false;
}

void SDigumWidgetStack::AddItemToStack(const TSharedPtr<SDigumWidget>& Item)
{
	if(Item == nullptr)
	{
		return;
	}
	
	int32 ZOrder = StackItems.Num()-1;
	
	if(DoesWidgetExist(Item))
	{
		RemoveFromStack_Internal(Item);
	}
	else
	{
		AddToStack_Internal(Item, ZOrder);
	}
}

void SDigumWidgetStack::AddDraggableItemToStack(const TSharedPtr<SDigumDragWidget>& Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding draggable item to stack"));
	DraggedWidget = Item;
	AddItemToStack(Item);
}

void SDigumWidgetStack::RemoveDraggedItemFromStack()
{
	RemoveFromStack_Internal(DraggedWidget);

	if(DraggedWidget.IsValid())
	{
		FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();
		TSharedPtr<SDigumWidget> OverlappedWidget;
		
		if(DoesOverlapAnyChildren(MousePosition, OverlappedWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("Mouse is over widget %s"), *OverlappedWidget->GetTypeAsString());
		}
		
	}

	
	DraggedWidget = nullptr;
}

void SDigumWidgetStack::RemoveLastItemFromStack()
{
	if(StackItems.Num() > 0)
	{
		RemoveFromStack_Internal(StackItems.Last());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
