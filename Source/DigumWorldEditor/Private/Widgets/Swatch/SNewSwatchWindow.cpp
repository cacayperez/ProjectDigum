// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SNewSwatchWindow.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "SSingleObjectDetailsPanel.h"
#include "DigumWorldEditorToolkit.h"
#include "Objects/DigumWorldEditorSwatch.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

class SDigumWorldEditorSwatchDetails : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SDigumWorldEditorSwatchDetails) {}
	SLATE_ATTRIBUTE(UDigumWorldEditorSwatch*, Swatch)
	SLATE_END_ARGS()

private:
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	TAttribute<UDigumWorldEditorSwatch*> Swatch;

public:
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
	{
		Swatch = InArgs._Swatch;
		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InToolkit->GetToolkitCommands()).HostTabManager(InToolkit->GetTabManager()), true, true);
	}

	virtual UObject* GetObjectToObserve() const override
	{
		return Swatch.Get();
	}

	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override
	{
		return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			PropertyEditorWidget
		];
	}
};

FReply SNewSwatchWindow::OnAddClicked()
{
	if(ToolkitPtr.Pin() != nullptr)
	{
		ToolkitPtr.Pin()->AddSwatchItem(Swatch.Get());
		OnAddSwatch.ExecuteIfBound();
	}
	return FReply::Handled();
}

void SNewSwatchWindow::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkitPtr)
{
	Swatch = InArgs._Swatch;
	SwatchItem = FDigumWorldSwatchPaletteItem();
	ToolkitPtr = InToolkitPtr;
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SDigumWorldEditorSwatchDetails, InToolkitPtr)
			.Swatch(Swatch.Get())
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Add"))
			.OnClicked(this, &SNewSwatchWindow::OnAddClicked)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


