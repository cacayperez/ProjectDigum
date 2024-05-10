// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindowHeader.h"
#include "Rendering/DrawElements.h"
#include "SlateOptMacros.h"
#include "Object/DigumWindow.h"
#include "Engine/Font.h"
#include "Core/SDigumWidgetStack.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumWindowHeader::Construct(const FArguments& InArgs)
{
	WindowStyle = InArgs._WindowStyle;
	SDigumWidget::Construct(
		SDigumWidget::FArguments()
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride));
	

}

void SDigumWindowHeader::OnConstruct()
{
	if(WindowStyle.Get() == nullptr) return;

	const FDigumFontSettings FontSettings= WindowStyle.Get()->HeaderFontSettings;
	FSlateFontInfo FontInfo;
	if(FontSettings.Font == nullptr)
	{
		FontInfo = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12);
	}
	else
	{
		FontInfo = FSlateFontInfo(FontSettings.Font, FontSettings.FontSize);
	}

	_Container->AddSlot()
	.VAlign(VAlign_Center)
	.Padding(5, 0)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(WindowStyle.Get()->TitleName)
			.Font(FontInfo)
			.ColorAndOpacity(WindowStyle.Get()->HeaderFontSettings.Color)
		]
		// TODO add minimize function later
		/*+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString("_"))
			.OnClicked_Lambda([&]()
			{
				OnMinimizeWindow.ExecuteIfBound();
				return FReply::Handled();
			})
		]*/
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString("X"))
			.OnClicked_Lambda([&]()
			{
				OnCloseWindow.ExecuteIfBound();
				return FReply::Handled();
			})
		]

	];
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
