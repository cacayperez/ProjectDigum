// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SBaseTab.h"

class FDigumWorldEditorToolkit;
class UDigumWorldAsset;

/**
 * 
 */
class DIGUMWORLDEDITOR_API SSwatchTab : public SBaseTab
{
public:
	SLATE_BEGIN_ARGS(SSwatchTab)
		{
		}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit);
	virtual void OnConstruct() override;
	void OnAddNewSwatch();	
	FReply OpenNewSwatchModal();
	void OnSelectSwatchIndex(const int32 InIndex);

protected:
	bool bHasWindowActive = false;
	TSharedPtr<SWindow> ModalWindowPtr;
	int32 GetActiveSwatchIndex() const;

public:
	void OnNewSwatchWindowClosed( const TSharedRef<SWindow>& Window);
	virtual void DrawTab() override;
};
