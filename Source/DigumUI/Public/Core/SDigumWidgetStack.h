// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDigumWidget;
/**
 * 
 */
class DIGUMUI_API SDigumWidgetStack : public SOverlay
{
public:
	template<typename T>
	static TSharedPtr<T> CreateWidget();

protected:
	TWeakObjectPtr<APlayerController> PlayerController;
	
	TArray<TSharedRef<SDigumWidget>> StackItems;
	
	void AddToStack_Internal(const TSharedPtr<SDigumWidget>& Item, const int32 ZOrder = 0);
	void RemoveFromStack_Internal(const TSharedPtr<SDigumWidget>& Item);
	void ClearStack_Internal();
	bool DoesWidgetExist(const TSharedPtr<SDigumWidget>& Item) const;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	
public:
	void AddItemToStack(const TSharedPtr<SDigumWidget>& Item);
	void RemoveLastItemFromStack();
	void SetPlayerController(APlayerController* Controller) { PlayerController = Controller; }
};

template <typename T>
TSharedPtr<T> SDigumWidgetStack::CreateWidget()
{
	TSharedPtr<T> WidgetPtr= SNew(T);
	return WidgetPtr;
}
