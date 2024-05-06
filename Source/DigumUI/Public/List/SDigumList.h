// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMUI_API SDigumList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumList){}
	SLATE_ATTRIBUTE(TArray<UObject*> , ListItems)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	TAttribute<TArray<UObject*>> _ListItemsAttribute;
	TSharedPtr<SGridPanel> _Container;
	
	virtual TSharedPtr<SWidget> CreateItem(UObject* Item) const;
	virtual TSharedPtr<SWidget> OnCreateItem(UObject* Item) const;
	virtual void PopulateItems() const;
public:
	void Refresh();
};
