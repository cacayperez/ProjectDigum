// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Window/SDigumWindow.h"

class UDigumWidget;
struct FDigumDragPayload;
class SDigumDragWidget;
class SDigumWidget;
/**
 * 
 */
class DIGUMUI_API SDigumWidgetStack : public SOverlay
{
public:
	~SDigumWidgetStack();
	
	template<typename T>
	static TSharedPtr<T> CreateWidget();

protected:
	
	TArray<TSharedRef<SDigumWidget>> StackItems;
	
	void AddToStack_Internal(const TSharedPtr<SDigumWidget>& Item, const int32 ZOrder = 0, EHorizontalAlignment HorizontalAlignment = HAlign_Center, EVerticalAlignment VerticalAlignment = VAlign_Center);
	void RemoveFromStack_Internal(const TSharedPtr<SDigumWidget>& Item);
	void ClearStack_Internal();
	bool DoesWidgetExist(const TSharedPtr<SDigumWidget>& Item) const;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	TSharedPtr<SDigumDragWidget> DraggedWidget;
	bool bHasDraggedWidget = false;

	bool DoesOverlapAnyChildren(const FVector2D& Position, TSharedPtr<SDigumWidget>& OutWidget) const;
	virtual bool IsInteractable() const override;
	virtual bool SupportsKeyboardFocus() const override;
public:
	void AddItemToStack(const TSharedPtr<SDigumWidget>& Item, EHorizontalAlignment HorizontalAlignment = HAlign_Center, EVerticalAlignment VerticalAlignment = VAlign_Center);
	void AddItemToStack(const UDigumWidget* WidgetObject, EHorizontalAlignment HorizontalAlignment = HAlign_Center, EVerticalAlignment VerticalAlignment = VAlign_Center);
	void RemoveWidget(const TSharedPtr<SDigumWidget>& Item);
	void AddDraggableItemToStack(const TSharedPtr<SDigumDragWidget>& Item);
	bool RemoveDraggedItemFromStack(UObject*& OutPayload);
	void RemoveLastItemFromStack();
	
};

template <typename T>
TSharedPtr<T> SDigumWidgetStack::CreateWidget()
{
	TSharedPtr<T> WidgetPtr= SNew(T);
	return WidgetPtr;
}
