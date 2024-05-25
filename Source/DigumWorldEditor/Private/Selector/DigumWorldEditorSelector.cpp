// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSelector.h"

#include "Asset/DigumWorldAsset.h"

TSharedPtr<SWidget> UDigumWorldEditorSelector::CreateSelectorWidget()
{
	return nullptr;
}

void UDigumWorldEditorSelector::AddSelection(FDigumWorldAssetCoordinate Coordinate)
{
	Selection.Add(Coordinate);
}

void UDigumWorldEditorSelector::ClearSelection()
{
	Selection.Empty();
}