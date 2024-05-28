// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SDigumWidget.h"
#include "Widgets/SCompoundWidget.h"

struct FDigumWindowStyle;
class SDigumWindowHeader;
class SDigumWindowContent;
class SDigumWindowContainer;
/**
 * 
 */
class DIGUMUI_API SDigumWindow : public SDigumWidget
{
private:
	FVector2D MousePosition;
	mutable FVector2D PreviousMousePosition;
	mutable FVector2D WindowPosition;

public:
	SLATE_BEGIN_ARGS(SDigumWindow){}
		SLATE_ATTRIBUTE(TSharedPtr<SDigumWidgetStack>, ParentContainer)
		SLATE_ATTRIBUTE(UMaterialInterface*, BackgroundMaterial)
		SLATE_ATTRIBUTE(FDigumWindowStyle*, WindowStyle)
		SLATE_ATTRIBUTE(float, HeightOverride)
		SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	virtual void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

protected:
	TAttribute<FDigumWindowStyle*> WindowStyle;
	FVector2D StartDrag;
	FVector2D DragMovePosition;

	bool bShowWindowHeader = false;
	bool bEnableDragWindow = false;
	bool bIsDraggingWindow = false;

	TSharedPtr<SWidget> WindowHeader;
	TSharedPtr<SWidget> WindowContent;
	
	virtual TSharedPtr<SWidget> OnCreateWindow();
	virtual TSharedPtr<SWidget> OnCreateContent();
	virtual TSharedPtr<SWidget> OnCreateHeader();

	virtual void DrawWindow();
public:

	void ToggleVisibility();
	void Refresh();
	void CloseWindow();
	
};
