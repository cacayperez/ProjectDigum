// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventorySlot.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"
#include "Core/SDigumWidgetStack.h"
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
	
	if(InventorySlot.IsValid())
	{
	
		if(InventorySlot->HasValidItem())
		{
			UTexture2D* Texture = InventorySlot->GetItemTexture();
			if(Texture)
			{
				FSlateImageBrush* Brush = new FSlateImageBrush(Texture, FVector2D(100.0f, 100.0f));
				_Container->AddSlot()
				[
					SNew(SImage)
					.Image(Brush)
				];
			}
			else
			{
                UE_LOG(LogTemp, Warning, TEXT("SDigumInventorySlot::OnConstruct - No Texture"));
            }
			
			_Container->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::FromName(InventorySlot->GetItemID()))
			];
			
			SetEnableDrag(true);
        	
			OnMouseDragStartDelegate.AddLambda([&](const FVector2D& Position)
			{
				if(InventoryWindow && InventorySlot.Get()) InventoryWindow->BeginDragItem(InventorySlot.Get());
			});
    
			OnMouseDragStopDelegate.AddLambda([&](const FVector2D& Position)
			{
				if(InventoryWindow) InventoryWindow->StopDragItem();
			});
		}
	
	}

	
	

}

void SDigumInventorySlot::OnReceiveDropPayload(UObject* InPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("SDigumInventorySlot::OnReceiveDropPayload"));
	if(InventorySlot.IsValid())
	{
		if(InventorySlot.Get() == InPayload) return;
		
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
