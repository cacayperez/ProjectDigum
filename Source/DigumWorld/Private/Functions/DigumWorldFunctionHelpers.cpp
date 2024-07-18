// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Functions/DigumWorldFunctionHelpers.h"

#include "Asset/DigumWorldBlockTable.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Settings/DigumWorldSettings.h"
#include "World/DigumWorldTypes.h"

/*
FVector UDigumWorldFunctionHelpers::GetGridSize()
{
	if(const UDigumWorldSettings* Sett)
}*/

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

TSubclassOf<ADigumWorldActorChild> UDigumWorldFunctionHelpers::GetChildActorClass(const FName& BlockID,
	const FName& ContentCategory)
{
	UDigumWorldSwatchAsset* SwatchAsset = GetSwatchAsset(BlockID, ContentCategory);

	if(SwatchAsset)
	{
		return SwatchAsset->GetChildActorClass();
	}
	return nullptr;
}

bool UDigumWorldFunctionHelpers::GetLocalSectionSize(const FName& InContentCategoryName, int32& OutWidth,
	int32& OutHeight)
{
	if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
	{
		const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(InContentCategoryName);
		if(Category)
		{
			OutWidth = Category->ProceduralRules.SectionWidth;
			OutHeight = Category->ProceduralRules.SectionHeight;
			return true;
		}
	}

	return false;
}

bool UDigumWorldFunctionHelpers::GetSectionCoordinates(const FName& InContentCategoryName, const FVector& InWorldLocation,
	int32& OutXCoordinate, int32& OutYCoordinate)
{
	if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
	{
		const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(InContentCategoryName);
		if(Category)
		{
			const FVector GridSize = GetDefault<UDigumWorldSettings>()->GetGridSize();
			const int32 SectionWidth = Category->ProceduralRules.SectionWidth;
			const int32 SectionHeight = Category->ProceduralRules.SectionHeight;
			const float UnitSectionWidth = SectionWidth * GridSize.X;
			const float UnitSectionHeight = SectionHeight * GridSize.Z;

			OutXCoordinate = FMath::FloorToInt(InWorldLocation.X / UnitSectionWidth);
			OutYCoordinate = -FMath::CeilToInt((InWorldLocation.Z) / UnitSectionHeight);

			return true;
		}
	}
	return false;
}

bool UDigumWorldFunctionHelpers::GetSectionCoordinates(const FName& InContentCategoryName,
	const TArray<FVector>& InWorldLocationArray, TArray<FDigumWorldProceduralSectionCoordinate>& OutSectionCoordinateArray)
{
	if(const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>())
	{
		const FDigumWorldContentCategory* Category = Settings->GetWorldContentCategoryData(InContentCategoryName);
		if(Category)
		{
			TArray<FDigumWorldProceduralSectionCoordinate> TempArray;
			const FVector GridSize = GetDefault<UDigumWorldSettings>()->GetGridSize();
			const int32 SectionWidth = Category->ProceduralRules.SectionWidth;
			const int32 SectionHeight = Category->ProceduralRules.SectionHeight;
			const float UnitSectionWidth = SectionWidth * GridSize.X;
			const float UnitSectionHeight = SectionHeight * GridSize.Z;

			for(const FVector& Location: InWorldLocationArray)
			{
				const int32 X = FMath::FloorToInt(Location.X / UnitSectionWidth);
				const int32 Y = -FMath::CeilToInt((Location.Z) / UnitSectionHeight);

				TempArray.Add(FDigumWorldProceduralSectionCoordinate(X, Y));
			}

			if(TempArray.Num() > 0)
			{
				OutSectionCoordinateArray = TempArray;
				return true;
			}
		}
	}
	return false;
}

void UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(const FVector& InWorldLocation,
	const FVector2D& InSectionUnitSize, FDigumWorldProceduralSectionCoordinate& OutSectionCoordinate)
{
	const int32 X = FMath::FloorToInt(InWorldLocation.X / InSectionUnitSize.X);
	const int32 Y = -FMath::CeilToInt((InWorldLocation.Z) / InSectionUnitSize.Y);

	const int32 AbsX = FMath::Abs(X) ;
	const int32 AbsY = FMath::Abs(Y);
	
	const FDigumWorldProceduralSectionCoordinate SectionCoordinate = FDigumWorldProceduralSectionCoordinate(AbsX, AbsY);
	OutSectionCoordinate = SectionCoordinate;
}

void UDigumWorldFunctionHelpers::ConvertToSectionLocalSectionCoordinate(const FVector& InWorldLocation,
	const FVector& InGridSize, const int32& InWidthOffset, const int32& InHeightOffset,
	FDigumWorldCoordinate2D& OutLocalCoordinate)
{
	const int32 X = InWorldLocation.X / InGridSize.X;
	const int32 Y = -(InWorldLocation.Z / InGridSize.Z);
	const int32 Hierarchy = -InWorldLocation.Y / InGridSize.Y;
	
	const int32 CoordinateX = FMath::Abs(X % InWidthOffset);
	const int32 CoordinateY = FMath::Abs(Y) > 0? Y % InWidthOffset : 0;

	OutLocalCoordinate = FDigumWorldCoordinate2D(CoordinateX, CoordinateY);
}