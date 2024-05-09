// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventoryDragWidget.h"

#include "SlateOptMacros.h"
#include "Components/DigumInventorySlot.h"
#include "Core/SDigumWidgetStack.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDigumInventoryDragWidget::OnSetDragPayload(UObject* InPayload)
{
	_Container->ClearChildren();
	if(InPayload == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SDigumInventoryDragWidget::OnSetDragPayload - Payload is null"));
		return;
	}
	
	UDigumInventorySlot* InventorySlot = Cast<UDigumInventorySlot>(InPayload);
	if(InventorySlot)
	{
		UTexture2D* Texture = InventorySlot->GetItemTexture();
		if(Texture)
		{
			FSlateImageBrush* TextureBrush = new FSlateImageBrush(Texture, FVector2D(100.0f, 100.0f));
			_Container->AddSlot()
			[
				SNew(SImage)
				.Image(TextureBrush)
			];
		}

		_Container->AddSlot()
		[
			SNew(STextBlock)
			.Text(FText::FromName(InventorySlot->GetItemID()))
		];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SDigumInventoryDragWidget::OnConstruct - No Inventory Slot Found"));
	}

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION