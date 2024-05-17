// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SBaseTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBaseTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	ToolkitPtr = InToolkit;
	_Container = SNew(SOverlay);

	if(ToolkitPtr.Pin())
	{
		if(UDigumWorldAsset* Asset = ToolkitPtr.Pin()->GetAssetBeingEdited())
		{
			Asset->OnDigumWorldAssetUpdated.AddSP(this, &SBaseTab::OnAssetUpdated);
		}
		
	}
	ChildSlot
	[
		_Container.ToSharedRef()
	];
	
	OnConstruct();
}

void SBaseTab::OnConstruct()
{
	DrawTab();	
}

UDigumWorldAsset* SBaseTab::GetAsset() const
{
	if(ToolkitPtr.Pin())
		return ToolkitPtr.Pin()->GetAssetBeingEdited();

	return nullptr;
}

void SBaseTab::OnAssetUpdated()
{
	RefreshTab();
}


void SBaseTab::DrawTab()
{
}

void SBaseTab::RefreshTab()
{
	DrawTab();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


