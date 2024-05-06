// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumItemTable.h"

#include "Asset/DigumItemAsset.h"

UDigumItemAsset* UDigumItemTable::GetDigumItemAsset(const FName& InItemID, UDataTable* InItemTable)
{
	FDigumItemTableRow ItemTableRow;

	if(GetItemTableRow(InItemID, InItemTable, ItemTableRow))
	{
		return ItemTableRow.ItemAsset.LoadSynchronous();
	}
	return nullptr;
}

int32 UDigumItemTable::GetDigumItemStackSize(const FName& InItemID, UDataTable* InItemTable)
{
	UDigumItemAsset* Asset = GetDigumItemAsset(InItemID, InItemTable);
	if(Asset)
	{
		return Asset->StackSize;
	}
	
	return -1;
}

bool UDigumItemTable::GetItemTableRow(const FName& InItemID, UDataTable* InItemTable, FDigumItemTableRow& OutItemTableRow)
{
	bool bResult = false;

	if(InItemTable)
	{
		FDigumItemTableRow* ItemTableRow = InItemTable->FindRow<FDigumItemTableRow>(InItemID, TEXT("Digum Item Get By ID"), true);
		if(ItemTableRow)
		{
			OutItemTableRow = *ItemTableRow;
			bResult = true;
		}
	}


	return bResult;
}
