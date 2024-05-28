// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FDigumDragPayload;
class UDigumWidgetStyle;
class SDigumWidgetStack;
/**
 * 
 */
class DIGUMUI_API SDigumWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumWidget)
		: _HeightOverride(100.0f)
		, _WidthOverride(100.0f)
	{}
	SLATE_ATTRIBUTE(TSharedPtr<SDigumWidgetStack>, ParentContainer)
	SLATE_ATTRIBUTE(UMaterialInterface*, BackgroundMaterial)
	SLATE_ATTRIBUTE(float, HeightOverride)
	SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()

	~SDigumWidget();
	
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct();
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnReceiveDropPayload(UObject* InPayload);
private:
	void StartDrag(const FVector2D& Position);
	void StopDrag(const FVector2D& Position);
	void MouseClickLeftDown();
	void MouseClickLeftUp();
	
protected:
	mutable FGeometry WidgetGeometry;
	// TSharedPtr<SOverlay> _BGContainer;
	TSharedPtr<SDigumWidgetStack> _Container;
	TSharedPtr<SDigumWidgetStack> _ParentContainer;
	
	TAttribute<float> HeightOverrideAttribute;
	TAttribute<float> WidthOverrideAttribute;
	TAttribute<UDigumWidgetStyle*> WidgetStyleClassAttribute;
	TAttribute<UMaterialInterface*> BackgroundMaterialAttribute;
	
	FVector2D PointerPosition;
	bool bHovered = false;
	bool bEnableDrag = false;
	bool bCanDrag = false;
	bool bIsDragging = false;
	
	DECLARE_MULTICAST_DELEGATE(FOnMouseClickLeftButton);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDigumMouseEvent, const FVector2D&);

	// Override with height and width attribute
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual TSharedPtr<SWidget> OnCreateBackground();

public:
	FOnMouseClickLeftButton OnMouseClickLeftButtonDownDelegate;
	FOnMouseClickLeftButton OnMouseClickLeftButtonUpDelegate;
	FOnDigumMouseEvent OnMouseDragStartDelegate;
	FOnDigumMouseEvent OnMouseDragStopDelegate;
	FOnDigumMouseEvent OnMouseDragMoveDelegate;

	void SetEnableDrag(bool EnableDrag) { bEnableDrag = EnableDrag; }
	bool IsDragEnabled() const { return bEnableDrag; }
	FGeometry GetWidgetGeometry() const { return WidgetGeometry; }
	void SetWidgetGeometry(const FGeometry& Geometry) const { WidgetGeometry = Geometry; }
	bool CanBeginDrag() const;
	
	TSharedPtr<SDigumWidgetStack> GetParentContainer() const { return _ParentContainer; }
	void SetParentContainer(TSharedPtr<SDigumWidgetStack> ParentContainer) { _ParentContainer = ParentContainer; }
	void ReceiveDropPayload(UObject* InPayload);
	
};
