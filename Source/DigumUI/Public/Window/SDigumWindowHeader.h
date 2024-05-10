// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"

struct FDigumWindowStyle;
/**
 * 
 */
class DIGUMUI_API SDigumWindowHeader : public SDigumWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumWindowHeader){}
		SLATE_ATTRIBUTE(FDigumWindowStyle*, WindowStyle)
		SLATE_ATTRIBUTE(float, HeightOverride)
		SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;

protected:
	DECLARE_DELEGATE(FOnCloseWindowDelegate)
	DECLARE_DELEGATE(FOnMinimizeWindowDelegate)
	
	TAttribute<FDigumWindowStyle*> WindowStyle;

public:
	FOnCloseWindowDelegate OnCloseWindow;
	FOnMinimizeWindowDelegate OnMinimizeWindow;
};
