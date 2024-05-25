// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMWORLDEDITOR_API SWidgetBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWidgetBase)
		{
		}

	SLATE_END_ARGS()

	virtual ~SWidgetBase() override;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct();
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

private:
	mutable bool bLeftMouseButtonHeld = false;
	
protected:
	bool bIsSelected = false;

	TSharedPtr<SOverlay> _Container;

	DECLARE_MULTICAST_DELEGATE(FOnSelectWidget);
	DECLARE_MULTICAST_DELEGATE(FOnDeselectWidget);
	DECLARE_MULTICAST_DELEGATE(FOnDoubleClickWidget);

public:
	FOnSelectWidget OnSelectWidget;
	FOnDeselectWidget OnDeselectWidget;
	FOnDoubleClickWidget OnDoubleClickWidget;

	void Select();
	void DoubleClick();
	void Deselect();
	bool IsSelected() const;
	void RefreshWidget();
};
