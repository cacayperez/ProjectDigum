// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SDigumInventoryWindow.h"

#include "SDigumInventoryItemList.h"
#include "SlateOptMacros.h"
#include "Components/DigumInventoryComponent.h"
#include "Components/DigumInventorySlot.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventoryWindow::Construct(const FArguments& InArgs)
{
	InventoryComponent = InArgs._InventoryComponent.Get();
	Height = 400;
	Width = 400;
	DrawWindow();
}

TArray<UObject*> SDigumInventoryWindow::GetInventoryItems() const
{
	if(InventoryComponent.IsValid())
	{
		TArray<UObject*> List;
		for(auto Item : InventoryComponent->GetInventoryItems())
		{
			List.Add(Item);
		}
		return List;
	}

	UE_LOG(LogTemp, Warning, TEXT("SDigumInventoryWindow::GetInventoryItems - InventoryComponent is not valid"));
	return {};
}

TSharedPtr<SWidget> SDigumInventoryWindow::OnCreateContent()
{
	ItemList = SNew(SDigumInventoryItemList)
		.ListItems(this, &SDigumInventoryWindow::GetInventoryItems);

	return ItemList;
}

void SDigumInventoryWindow::SetInventoryComponent(UDigumInventoryComponent* Component)
{
	const TWeakObjectPtr<UDigumInventoryComponent> WeakComp = Component;
	InventoryComponent = WeakComp;
	
	Refresh();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

