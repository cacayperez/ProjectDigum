// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "List/SDigumList.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumList::Construct(const FArguments& InArgs)
{
	_ListItemsAttribute = InArgs._ListItems;
	ChildSlot
	[
		SAssignNew(_Container, SGridPanel)
	];

	PopulateItems();
}

TSharedPtr<SWidget> SDigumList::CreateItem(UObject* Item) const
{
	return OnCreateItem(Item);
}

TSharedPtr<SWidget> SDigumList::OnCreateItem(UObject* Item) const
{
	return nullptr;
}

void SDigumList::PopulateItems() const
{
	_Container->ClearChildren();

	TArray<UObject*> Items = _ListItemsAttribute.Get();

	// TODO: Make this dynamic
	int32 Row = 0;
	int32 Column = 0;
	int32 MaxRows = 3;
	int32 Iterator = 0;
	float Padding = 10.0f;
	
	for(UObject* Item : Items)
	{
		Column = Iterator / MaxRows;
		Row = Iterator % MaxRows;
		
		_Container->AddSlot(Row, Column)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(Padding)
		[
			CreateItem(Item).ToSharedRef()
		];
		Iterator++;
	}
}

void SDigumList::Refresh()
{
	PopulateItems();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
