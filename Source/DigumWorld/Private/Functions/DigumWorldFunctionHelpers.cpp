// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Functions/DigumWorldFunctionHelpers.h"

#include "Asset/DigumWorldBlockTable.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Settings/DigumWorldSettings.h"

UDigumWorldSwatchAsset* UDigumWorldFunctionHelpers::GetSwatchAsset(const FName& BlockID, const FName& ContentCategory)
{
	if(UDataTable* Table = GetSwatchDataTable(ContentCategory))
	{
		const FDigumWorldBlockTableRow* BlockTableRow = GetBlockTableRow(BlockID, Table);

		if(BlockTableRow)
		{
			return UDigumAssetManager::GetAsset<UDigumWorldSwatchAsset>(BlockTableRow->SwatchAsset);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSwatchAsset: Could not find table for category %s"), *ContentCategory.ToString());
	
	}
	return nullptr;
}

FDigumWorldBlockTableRow* UDigumWorldFunctionHelpers::GetBlockTableRow(const FName& BlockID, UDataTable* InTable)
{
	if(InTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetBlockTableRow: Table is null"));
		return nullptr;
	}
	
	return InTable->FindRow<FDigumWorldBlockTableRow>(BlockID, TEXT("Digum World Block Get By ID"), true);
}

UDataTable* UDigumWorldFunctionHelpers::GetSwatchDataTable(const FName ContentCategory)
{
	if(const FDigumWorldContentCategory* ContentCategoryData = UDigumWorldSettings::GetWorldContentCategoryData(ContentCategory))
	{
		if(ContentCategoryData == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetSwatchDataTable: Could not find category %s"), *ContentCategory.ToString());
		}
		return UDigumAssetManager::GetAsset<UDataTable>(ContentCategoryData->BlockTable);
	}
		
	return nullptr;
}
