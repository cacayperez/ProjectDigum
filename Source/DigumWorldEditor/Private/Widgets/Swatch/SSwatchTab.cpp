// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SSwatchTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "SNewSwatchWindow.h"
#include "SSwatchItem.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Objects/DigumWorldEditorSwatch.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSwatchTab::Construct(const FArguments& InArgs,  TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}
FReply SSwatchTab::OpenNewSwatchModal()
{
	if(bHasWindowActive)
	{
		return FReply::Handled();
	}
	TSharedPtr<FDigumWorldEditorToolkit> Toolkit = ToolkitPtr.Pin();
	UDigumWorldEditorSwatch* SwatchObject = NewObject<UDigumWorldEditorSwatch>();
	TSharedPtr<SNewSwatchWindow> WindowContent =
	SNew(SNewSwatchWindow, Toolkit)
	.Swatch(SwatchObject);
	
	WindowContent->OnAddSwatch.BindSP(this, &SSwatchTab::OnAddNewSwatch);
	
	TSharedPtr<SWindow> WindowContainer = SNew(SWindow)
	.Title(FText::FromString("New Swatch"))
	.ClientSize(FVector2D(400, 400));

	WindowContainer->SetContent(
		SNew(SBox)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				WindowContent.ToSharedRef()
			]
		]
	);
	ModalWindowPtr = FSlateApplication::Get().AddWindow(WindowContainer.ToSharedRef());
	ModalWindowPtr->SetOnWindowClosed(FOnWindowClosed::CreateSP(this, &SSwatchTab::OnNewSwatchWindowClosed));
	
	bHasWindowActive = true;

	
	return FReply::Handled();
}

void SSwatchTab::OnConstruct()
{
	SBaseTab::OnConstruct();
}

void SSwatchTab::OnAddNewSwatch()
{
	if(ModalWindowPtr)
	{
		FSlateApplication::Get().RequestDestroyWindow(ModalWindowPtr.ToSharedRef());
		bHasWindowActive = false;
	}
}

void SSwatchTab::OnSelectSwatchIndex(const int32 InIndex)
{
	SelectedSwatcIndex = InIndex;
	UE_LOG(LogTemp, Warning, TEXT("Selected Index %i"), InIndex);
	RefreshTab();
}
/*
SSwatchTab::~SSwatchTab()
{
	NewSwatchWindow.Reset();
}
*/


void SSwatchTab::OnNewSwatchWindowClosed(const TSharedRef<SWindow>& Window)
{
	ModalWindowPtr = nullptr;
	bHasWindowActive = false;
}

void SSwatchTab::DrawTab()
{
	_Container->ClearChildren();
	TSharedPtr<SGridPanel> GridPanel = SNew(SGridPanel);

	int32 Width = 8;
	TArray<FDigumWorldSwatchPaletteItem> Swatches =  ToolkitPtr.Pin()->GetAssetBeingEdited()->GetSwatches();
	for(int32 i = 0; i < Swatches.Num(); i++)
	{
		FDigumWorldSwatchPaletteItem Swatch = Swatches[i];
		
		UDigumWorldSwatchAsset* SwatchAsset = Swatch.SoftSwatchAsset.LoadSynchronous();
		if(SwatchAsset)
		{
			bool bIsActive = (i == SelectedSwatcIndex);
			TSharedPtr<SSwatchItem> SwatchItem
				= SNew(SSwatchItem)
				.SwatchAsset(SwatchAsset)
				.bIsActiveSwatch(bIsActive);
			
			SwatchItem->OnSelectWidget.AddLambda([this, i]()
			{
				OnSelectSwatchIndex(i);
			});
			
			int32 X = i % Width;
			int32 Y = i / Width;
			GridPanel->AddSlot(X, Y)
			.Padding(2.0f)
			[
				SwatchItem.ToSharedRef()
			];
		
		}
	}
	
	_Container->AddSlot()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			GridPanel.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("New Swatch"))
			.OnClicked(this, &SSwatchTab::OpenNewSwatchModal)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
