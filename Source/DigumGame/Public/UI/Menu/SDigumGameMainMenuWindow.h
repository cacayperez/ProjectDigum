// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

struct FDigumWindowStyle;;



/**
 * 
 */
class DIGUMGAME_API SDigumGameMainMenuWindow : public SDigumWindow
{
public:
	UENUM()
	enum EDigumGameMainMenuWindowType : uint8
	{
		DigumGameMenu_Default,
		DigumGameMenu_NewWorld,
		DigumGameMenu_LoadWorld
	};
	
	SLATE_BEGIN_ARGS(SDigumGameMainMenuWindow) {}
		SLATE_ATTRIBUTE(TSharedPtr<SDigumWidgetStack>, ParentContainer)
		SLATE_ATTRIBUTE(UMaterialInterface*, BackgroundMaterial)
		SLATE_ATTRIBUTE(FDigumWindowStyle*, WindowStyle)
		SLATE_ATTRIBUTE(float, HeightOverride)
		SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:
	TEnumAsByte<EDigumGameMainMenuWindowType> WindowType = EDigumGameMainMenuWindowType::DigumGameMenu_Default;

	virtual TSharedPtr<SWidget> OnCreateWindow() override;
	virtual TSharedPtr<SWidget> OnCreateContent() override;
	virtual void AddWindowContentToSlot(TSharedPtr<SVerticalBox>& SlotContainer) override;

	TSharedPtr<SWidget> CreateDefaultMenu();

	TSharedPtr<SWidget> CreateNewWorldMenu();

	TSharedPtr<SWidget> CreateLoadWorldMenu();

	void SetWindowType(EDigumGameMainMenuWindowType InWindowType);
};
