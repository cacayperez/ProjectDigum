// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Object/DigumWidget.h"
#include "Core/SDigumWidget.h"


UDigumWidget::UDigumWidget()
{
	ConstructParameters.Size = FVector2D(100, 100);
}

UDigumWidget::~UDigumWidget()
{
	Widget.Reset();
}
void UDigumWidget::OnCreateWidget()
{
	float Width = ConstructParameters.Size.X;
	float Height = ConstructParameters.Size.Y;
	
	Widget = SNew(SDigumWidget)
		.BackgroundMaterial(BackgroundMaterial)
		.HeightOverride(Height)
		.WidthOverride(Width);
}

TSharedPtr<SDigumWidget> UDigumWidget::GetWidget() const
{
	return Widget;
}