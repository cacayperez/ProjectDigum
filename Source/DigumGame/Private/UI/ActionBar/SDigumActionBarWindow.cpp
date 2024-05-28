// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/ActionBar/SDigumActionBarWindow.h"

#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "Core/SDigumWidgetStack.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


// overriden for custom alignment
TSharedPtr<SWidget> SDigumActionBarWindow::OnCreateWindow()
{
	TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
	
	if(bShowWindowHeader)
	{
		VerticalBox->AddSlot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			OnCreateHeader().ToSharedRef()
		];
	}
	
	VerticalBox->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	.FillHeight(1.0)
	[
		OnCreateContent().ToSharedRef()
	];
	
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.VAlign(VAlign_Bottom)
	[
		VerticalBox.ToSharedRef()
	];
}

TSharedPtr<SWidget> SDigumActionBarWindow::OnCreateBackground()
{
	TSharedPtr<SOverlay> Widget = SNew(SOverlay);

	UMaterialInterface* Material = BackgroundMaterialAttribute.Get();
	const FSlateMaterialBrush* MaterialBrush = new FSlateMaterialBrush(*Material, FVector2D(WidthOverrideAttribute.Get(), HeightOverrideAttribute.Get()));

	if(Material && MaterialBrush)
	{
		Widget->AddSlot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Center)
		[
			SNew(SImage)
			.Image(MaterialBrush)
		];
	}

	return Widget;
}

void SDigumActionBarWindow::OnReceiveDropPayload(UObject* InPayload)
{
	SDigumInventoryWindow::OnReceiveDropPayload(InPayload);
	UE_LOG(LogTemp, Warning, TEXT("SDigumActionBarWindow::OnReceiveDropPayload - Not Implemented"));
}

void SDigumActionBarWindow::DrawWindow()
{
	_Container->ClearChildren();
	_Container->AddSlot()
	[
		OnCreateWindow().ToSharedRef()
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
