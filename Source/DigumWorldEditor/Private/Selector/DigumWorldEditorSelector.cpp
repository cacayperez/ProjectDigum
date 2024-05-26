// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSelector.h"

#include "Asset/DigumWorldAsset.h"


TSharedPtr<SWidget> UDigumWorldEditorSelector::CreateSelectorButtonWidget()
{
	return nullptr;
}

TSharedPtr<SWidget> UDigumWorldEditorSelector::CreateSelectorSelectionWidget()
{
	return nullptr;
}

void UDigumWorldEditorSelector::AddSelection(FDigumWorldAssetCoordinate Coordinate)
{
	Selection.AddCoordinate(Coordinate);
	OnSetSelectionDelegate.Broadcast(Coordinate);
	EndSelection();
}

void UDigumWorldEditorSelector::ClearSelection()
{
	Selection.Clear();
}

void UDigumWorldEditorSelector::EndSelection()
{
	OnEndSelectionDelegate.Broadcast();
}

void UDigumWorldEditorSelector::BeginSelection()
{
}
