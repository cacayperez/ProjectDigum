// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventorySlot.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"
#include "Core/SDigumWidgetStack.h"
#include "UI/Inventory/SDigumInventorySlotContent.h"
#include "UI/Inventory/SDigumInventoryWindow.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventorySlot::Construct(const FArguments& InArgs)
{
	InventorySlot = InArgs._InventorySlot.Get();
	
	SDigumWidget::Construct(SDigumWidget::FArguments()
	.HeightOverride(InArgs._HeightOverride)
	.WidthOverride(InArgs._WidthOverride));
}

void SDigumInventorySlot::OnConstruct()
{
	SetEnableDrag(true);
	if(InventorySlot.IsValid())
	{
		_Container->AddSlot()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromName(InventorySlot->GetItemID()))
			]
			
		];
	}

	OnMouseDragStartDelegate.AddLambda([&](const FVector2D& Position)
	{
		
		if(InventoryWindow && InventorySlot.Get()) InventoryWindow->BeginDragItem(InventorySlot.Get());
	});

	OnMouseDragStopDelegate.AddLambda([&](const FVector2D& Position)
	{
		if(InventoryWindow) InventoryWindow->StopDragItem();
	});
}

void SDigumInventorySlot::OnReceiveDropPayload(UObject* InPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("SDigumInventorySlot::OnReceiveDropPayload"));
	if(InventorySlot.IsValid())
	{
		if(UDigumInventorySlot* OtherSlot = Cast<UDigumInventorySlot>(InPayload))
		{
			InventorySlot->SwapContent(OtherSlot);
		}
	}
}

void SDigumInventorySlot::SetInventoryWindow(const TSharedPtr<SDigumInventoryWindow>& Window)
{
	InventoryWindow = Window;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
