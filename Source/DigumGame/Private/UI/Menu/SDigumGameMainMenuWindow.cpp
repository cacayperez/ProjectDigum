// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Menu/SDigumGameMainMenuWindow.h"

#include "SlateOptMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Menu/SDigumGameMainMenu_NewWorldForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumGameMainMenuWindow::Construct(const FArguments& InArgs)
{
	SDigumWindow::Construct(SDigumWindow::FArguments()
		.WindowStyle(InArgs._WindowStyle)
		.BackgroundMaterial(InArgs._BackgroundMaterial)
		.ParentContainer(InArgs._ParentContainer)
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride));
}

TSharedPtr<SWidget> SDigumGameMainMenuWindow::OnCreateWindow()
{
	return SDigumWindow::OnCreateWindow();
}

TSharedPtr<SWidget> SDigumGameMainMenuWindow::OnCreateContent()
{
	TSharedPtr<SBox> Box = SNew(SBox);
	
	switch (WindowType)
	{
	case EDigumGameMainMenuWindowType::DigumGameMenu_Default:
		return SNew(SBox)
		[
			CreateDefaultMenu().ToSharedRef()
		];
	case EDigumGameMainMenuWindowType::DigumGameMenu_NewWorld:
		return SNew(SBox)
		[
			CreateNewWorldMenu().ToSharedRef()
		];
	case EDigumGameMainMenuWindowType::DigumGameMenu_LoadWorld:
		return SNew(SBox)
		[
			CreateLoadWorldMenu().ToSharedRef()
		];
		break;
	default:
		break;
	}
	
	return SNew(SBox)
	[
		CreateDefaultMenu().ToSharedRef()
	];
}

void SDigumGameMainMenuWindow::AddWindowContentToSlot(TSharedPtr<SVerticalBox>& SlotContainer)
{
	// Override fill alignment
	if(SlotContainer)
	{
		SlotContainer->AddSlot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.FillHeight(1.0)
		[
			OnCreateContent().ToSharedRef()
		];
	}
}

TSharedPtr<SWidget> SDigumGameMainMenuWindow::CreateDefaultMenu()
{
	const float OptionPadding = 20.0f;
	TSharedPtr<SOverlay> Overlay = SNew(SOverlay);
	
	TSharedPtr<SVerticalBox> Box = SNew(SVerticalBox);

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(OptionPadding)
		[
			SNew(SButton)
			.Text(FText::FromString("Create World"))
			.HAlign(HAlign_Center)
			.OnClicked_Lambda([this]()
			{
			   SetWindowType(EDigumGameMainMenuWindowType::DigumGameMenu_NewWorld);
			   return FReply::Handled();
			})
		]
	];

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(OptionPadding)
		[
			SNew(SButton)
			.Text(FText::FromString("Load World"))
			.HAlign(HAlign_Center)
			.OnClicked_Lambda([this]()
			{
			   SetWindowType(EDigumGameMainMenuWindowType::DigumGameMenu_LoadWorld);
			   return FReply::Handled();
			})
		]
	];

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(OptionPadding)
		.HAlign(HAlign_Center)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.Text(FText::FromString("Exit"))
			.OnClicked_Lambda([this]()
			{
				if (GEngine && GEngine->GetWorld())
				{
					UWorld* World = GEngine->GetWorld();
					UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("No world found"));
				}
				return FReply::Handled();
			})
		]
	];

	Overlay->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SNew(SBox)
		.WidthOverride(WidthOverrideAttribute.Get())
		[
			Box.ToSharedRef()
		]
	];

	return Overlay;
}

TSharedPtr<SWidget> SDigumGameMainMenuWindow::CreateNewWorldMenu()
{
	const float OptionPadding = 20.0f;
	TSharedPtr<SOverlay> Overlay = SNew(SOverlay);
	
	TSharedPtr<SVerticalBox> Box = SNew(SVerticalBox);

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(OptionPadding)
		[
			SNew(SVerticalBox)
			// Title Slot
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Create World"))
			]
		]
	];

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.Padding(OptionPadding)
		[
			SNew(SDigumGameMainMenu_NewWorldForm)
		]
	];

	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.Padding(OptionPadding)
		[
			SNew(SButton)
			.Text(FText::FromString("Back"))
			.OnClicked_Lambda([this]()
			{
				SetWindowType(EDigumGameMainMenuWindowType::DigumGameMenu_Default);
				return FReply::Handled();
			})
		]
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.Padding(OptionPadding)
		[
			SNew(SButton)
			.Text(FText::FromString("Create"))
			.OnClicked_Lambda([this]()
			{
				// SetWindowType(EDigumGameMainMenuWindowType::DigumGameMenu_Default);
				return FReply::Handled();
			})
		]
	];
	
	Overlay->AddSlot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SNew(SBox)
		.WidthOverride(WidthOverrideAttribute.Get())
		[
			Box.ToSharedRef()
		]
	];

	return Overlay;
}

TSharedPtr<SWidget> SDigumGameMainMenuWindow::CreateLoadWorldMenu()
{
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Load World"))
	]
	+ SVerticalBox::Slot()
	[
		SNew(SButton)
		.Text(FText::FromString("Back"))
		.OnClicked_Lambda([this]()
		{
			SetWindowType(EDigumGameMainMenuWindowType::DigumGameMenu_Default);
			return FReply::Handled();
		})
	];
}

void SDigumGameMainMenuWindow::SetWindowType(EDigumGameMainMenuWindowType InWindowType)
{
	WindowType = InWindowType;
	Refresh();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
