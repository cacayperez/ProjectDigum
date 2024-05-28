// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SBaseTab.h"

class FDigumWorldEditorToolkit;
class UDigumWorldAsset;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SLayerTab : public SBaseTab
{
public:
	SLATE_BEGIN_ARGS(SLayerTab)
		{
		}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
	void OnSetLayerName(const int32& InLayerIndex, const FText& Text);
	void OnSetLayerVisibility(const int32& InLayerIndex, const bool& bInVisibility);
	void SwapLayers(const int32& InLayerIndexA, const int32& InLayerIndexB);

protected:
	TSharedPtr<SWidget> OnCreateLayerMenu();
	void OnLayerNameCommitted(const FText& Text, ETextCommit::Type Arg, int InIndex);
	int32 GetActiveLayerIndex() const;
	TSharedPtr<SWidget> OnCreateLayerList();

	void AddNewLayer();
	void OnSelectActiveLayerIndex(const int32 InIndex);
	
public:
	virtual void DrawTab() override;
	void DeleteSelectedLayer();	
	// void UpdateLayer(const int32& InLayerIndex, const FDigumWorldAssetLayer& Layer);
};
