// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnMouseDragStart, const FVector2D&);
DECLARE_DELEGATE_OneParam(FOnMouseDragStop, const FVector2D&);
DECLARE_DELEGATE_OneParam(FOnMouseDragMove, const FVector2D&);
/**
 * 
 */
class DIGUMUI_API SDigumWindowHeader : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumWindowHeader)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
protected:
	bool bCanDrag = false;
	bool bIsDragging = false;


public:
	FOnMouseDragStart OnMouseDragStartDelegate;
	FOnMouseDragStop OnMouseDragStopDelegate;
	FOnMouseDragMove OnMouseDragMoveDelegate;
};
