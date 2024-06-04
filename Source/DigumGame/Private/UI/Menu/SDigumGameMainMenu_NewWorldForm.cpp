// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "UI/Menu/SDigumGameMainMenu_NewWorldForm.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FText SDigumGameMainMenu_NewWorldForm::GetWorldNameText() const
{
	return WorldName;
}

void SDigumGameMainMenu_NewWorldForm::OnWorldNameChanged(const FText& Text)
{
}

void SDigumGameMainMenu_NewWorldForm::OnWorldNameCommitted(const FText& Text, ETextCommit::Type Arg)
{
	WorldName = Text;
}

FText SDigumGameMainMenu_NewWorldForm::GetWorldSeedText() const
{
	return FText::FromString(FString::FromInt(CurrentSeed));
}

void SDigumGameMainMenu_NewWorldForm::OnWorldSeedChanged(const FText& Text)
{
	/*FString String = Text.ToString();
	if (!String.IsNumeric())
	{
		CurrentSeed = FMath::Rand();
	}*/
}

void SDigumGameMainMenu_NewWorldForm::OnWorldSeedCommitted(const FText& Text, ETextCommit::Type Arg)
{
	FString String = Text.ToString();
	if (!String.IsNumeric())
	{
		CurrentSeed = FMath::Rand();
	}
	else
	{
		CurrentSeed = FCString::Atoi(*Text.ToString());
	}
	
}

void SDigumGameMainMenu_NewWorldForm::Construct(const FArguments& InArgs)
{

	TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
	const float InputWidth = 400.0f;

	CurrentSeed = FMath::Rand();
	
	VerticalBox->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		[
			SNew(STextBlock)
			.Text(FText::FromString("World Name"))
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SBox)
			.WidthOverride(InputWidth)
			[
				SNew(SEditableTextBox)
				.Text(this, &SDigumGameMainMenu_NewWorldForm::GetWorldNameText)
				.OnTextChanged(this, &SDigumGameMainMenu_NewWorldForm::OnWorldNameChanged)
				.OnTextCommitted(this, &SDigumGameMainMenu_NewWorldForm::OnWorldNameCommitted)
			]
		]
		
	];

	VerticalBox->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Seed"))
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SBox)
			.WidthOverride(InputWidth)
			[
				SNew(SEditableTextBox)
				.Text(this, &SDigumGameMainMenu_NewWorldForm::GetWorldSeedText)
				.OnTextChanged(this, &SDigumGameMainMenu_NewWorldForm::OnWorldSeedChanged)
				.OnTextCommitted(this, &SDigumGameMainMenu_NewWorldForm::OnWorldSeedCommitted)
			]
		]
		
	];

	/*VerticalBox->AddSlot()
	.AutoHeight()
	[
		CreateInputRow(FText::FromString("World Seed"), InputWidth, WorldSeedText, OnWorldSeedChanged, OnWorldSeedCommitted).ToSharedRef()
	];*/

	VerticalBox->AddSlot()
	.AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SSpacer)
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Randomize Seed"))
			.OnClicked_Lambda([this]() {
				CurrentSeed = FMath::Rand();
				return FReply::Handled();
			})
		]
	];
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		[
			VerticalBox.ToSharedRef()
		]
	];
	
}

TSharedPtr<SWidget> SDigumGameMainMenu_NewWorldForm::CreateInputRow(const FText& LabelText, const float& InputWidth, const FText& InputText, const FOnTextChanged& OnTextChange, const FOnTextCommitted& OnTextCommitted)
{
	return SNew(SHorizontalBox)
	+ SHorizontalBox::Slot()
	.HAlign(HAlign_Right)
	[
		SNew(STextBlock)
		.Text(LabelText)
	]
	+ SHorizontalBox::Slot()
	[
		SNew(SBox)
		.WidthOverride(InputWidth)
		[
			SNew(SEditableTextBox)
			.Text(InputText)
			.OnTextChanged(OnTextChange)
			.OnTextCommitted(OnTextCommitted)
		]
	];
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
