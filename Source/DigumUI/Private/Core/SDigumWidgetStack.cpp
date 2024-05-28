// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Core/SDigumWidgetStack.h"
#include "Core/SDigumWidget.h"
#include "Drag/SDigumDragWidget.h"
#include "SlateOptMacros.h"
#include "Object/DigumWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SDigumWidgetStack::~SDigumWidgetStack()
{
	DraggedWidget = nullptr;
	StackItems.Empty();
}

void SDigumWidgetStack::AddToStack_Internal(const TSharedPtr<SDigumWidget>& Item, const int32 ZOrder, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment)
{
	const TSharedRef<SDigumWidget> Shared = Item.ToSharedRef();
	
	AddSlot(ZOrder)
	.HAlign(HorizontalAlignment)
	.VAlign(VerticalAlignment)
	[
		Shared
	];
	Shared->SetParentContainer(SharedThis(this));
	StackItems.Add(Shared);
}

void SDigumWidgetStack::RemoveFromStack_Internal(const TSharedPtr<SDigumWidget>& Item)
{
	if(Item == nullptr) return;
	
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
	if(StackItems.IsEmpty()) return false;

	// Start loop at the end of the array (top of the stack)
    const int32 LastIndex = (StackItems.Num() - 1 >= 0) ? StackItems.Num() - 1 : 0;
	for(int32 i = LastIndex; i >= 0; i--)
	{
		const TSharedRef<SDigumWidget>& Item = StackItems[i];
		if(Item->GetWidgetGeometry().IsUnderLocation(Position))
		{
			OutWidget = Item;
			return true;
		}
	}
	return false;
}

bool SDigumWidgetStack::IsInteractable() const
{
	return true;
}

bool SDigumWidgetStack::SupportsKeyboardFocus() const
{
	return true;
}

void SDigumWidgetStack::AddItemToStack(const TSharedPtr<SDigumWidget>& Item, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment)
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
		AddToStack_Internal(Item, ZOrder, HorizontalAlignment, VerticalAlignment);
	}
}

void SDigumWidgetStack::AddItemToStack(const UDigumWidget* WidgetObject, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment)
{
	if(WidgetObject == nullptr)
	{
		return;
	}

	AddItemToStack(WidgetObject->GetWidget(), HorizontalAlignment, VerticalAlignment);
}

void SDigumWidgetStack::RemoveWidget(const TSharedPtr<SDigumWidget>& Item)
{
	RemoveFromStack_Internal(Item);
}

void SDigumWidgetStack::AddDraggableItemToStack(const TSharedPtr<SDigumDragWidget>& Item)
{
	if(!bHasDraggedWidget)
	{
		bHasDraggedWidget = true;
		DraggedWidget = Item;
		AddItemToStack(Item, EHorizontalAlignment::HAlign_Center, EVerticalAlignment::VAlign_Center);
	}
}

bool SDigumWidgetStack::RemoveDraggedItemFromStack(UObject*& OutPayload)
{
	RemoveFromStack_Internal(DraggedWidget);
	bHasDraggedWidget = false;
	if(DraggedWidget.IsValid())
	{
		FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();
		TSharedPtr<SDigumWidget> OverlappedWidget;
		OutPayload = DraggedWidget->GetDragPayload();
		
		if(DoesOverlapAnyChildren(MousePosition, OverlappedWidget))
		{
			OverlappedWidget->ReceiveDropPayload(OutPayload);
			return true;
		}
	}

	
	DraggedWidget = nullptr;
	return false;
}

void SDigumWidgetStack::RemoveLastItemFromStack()
{
	if(StackItems.Num() > 0)
	{
		RemoveFromStack_Internal(StackItems.Last());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
