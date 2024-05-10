// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Inventory/DigumInventoryWidget.h"

#include "UI/Inventory/SDigumInventoryWindow.h"


UDigumInventoryWidget::~UDigumInventoryWidget()
{
	InventorySlate.Reset();
}

void UDigumInventoryWidget::OnCreateWidget()
{
	const float Width = ConstructParameters.Size.X;
	const float Height = ConstructParameters.Size.Y;

	UE_LOG(LogTemp, Warning, TEXT("UDigumInventoryWidget::OnCreateWidget, Width: %f, Height: %f"), Width, Height);
	InventorySlate =
		SNew(SDigumInventoryWindow)
		.BackgroundMaterial(BackgroundMaterial)
		.HeightOverride(Height)
		.WidthOverride(Width) ;
}

TSharedPtr<SDigumWidget> UDigumInventoryWidget::GetWidget() const
{
	return InventorySlate;
}

void UDigumInventoryWidget::SetInventoryComponent(UDigumGameInventoryComponent* InventoryComponent)
{
	if(InventorySlate)
		InventorySlate->SetInventoryComponent(InventoryComponent);
}
