// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/SDigumInventorySlotContent.h"

#include "SlateOptMacros.h"
#include "Core/SDigumWidgetStack.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumInventorySlotContent::Construct(const FArguments& InArgs)
{
	SDigumWidget::Construct(SDigumWidget::FArguments()
	.HeightOverride(InArgs._HeightOverride)
	.WidthOverride(InArgs._WidthOverride));

}

int32 SDigumInventorySlotContent::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FVector2D GeometrySize = AllottedGeometry.GetLocalSize();
	
	FGeometry NewGeometry = AllottedGeometry.MakeChild(GeometrySize, FSlateLayoutTransform( ));
	// Draw the background
	FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			NewGeometry.ToPaintGeometry(),
			FCoreStyle::Get().GetBrush("WhiteBrush"),  // Use a default brush
			ESlateDrawEffect::None,
			FLinearColor::Red
		);
	return SDigumWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                             bParentEnabled);
}

void SDigumInventorySlotContent::OnConstruct()
{
	_Container->AddSlot()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Hello")))
	];

	SetEnableDrag(true);

	OnMouseDragStartDelegate.AddLambda([&](const FVector2D& MousePosition)
	{
		// TODO: Implement Drag and Drop
		/*TSubclassOf<UDragDropOperation> DragDropClass = UDigumGameDeveloperSettings::GetDragDropOperationClass();
		TSubclassOf<UUserWidget> DragWidgetClass = UDigumGameDeveloperSettings::GetDragDropWidgetClass();

		if(DragDropClass && DragWidgetClass && GEngine)
		{
			UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>(InventorySlot.Get(), DragDropClass);
			UUserWidget* Widget = CreateWidget<UUserWidget>(InventorySlot->GetOwningInventory()->GetWorld(), DragWidgetClass);
			
			DragDropOp->Payload = InventorySlot.Get();
			DragDropOp->DefaultDragVisual = Widget;
			GEngine->GameViewport->AddViewportWidgetContent(Widget->TakeWidget());

			UE_LOG(LogTemp, Warning, TEXT("I should be working"));
		}*/
	});
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
