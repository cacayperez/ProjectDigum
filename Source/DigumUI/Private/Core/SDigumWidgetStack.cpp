// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Core/SDigumWidgetStack.h"
#include "Core/SDigumWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumWidgetStack::AddToStack_Internal(const TSharedPtr<SDigumWidget>& Item, const int32 ZOrder)
{
	const TSharedRef<SDigumWidget> Shared = Item.ToSharedRef();
	
	AddSlot(ZOrder)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		Shared
	];

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
	StackItems.Empty();
}

bool SDigumWidgetStack::DoesWidgetExist(const TSharedPtr<SDigumWidget>& Item) const
{
	return StackItems.Contains(Item.ToSharedRef());
}

void SDigumWidgetStack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SOverlay::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();

	for(const TSharedRef<SDigumWidget>& Item : StackItems)
	{
		if(Item->GetWidgetGeometry().IsUnderLocation(MousePosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Mouse is over widget %s"), *Item->GetTypeAsString());
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("NOT"));
	}
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

void SDigumWidgetStack::RemoveLastItemFromStack()
{
	if(StackItems.Num() > 0)
	{
		RemoveFromStack_Internal(StackItems.Last());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
