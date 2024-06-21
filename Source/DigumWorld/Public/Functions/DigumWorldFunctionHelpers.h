// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldFunctionHelpers.generated.h"

struct FDigumWorldProceduralSectionCoordinate;
class ADigumWorldActorChild;
struct FDigumWorldBlockTableRow;
class UDigumWorldSwatchAsset;
/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldFunctionHelpers : public UObject
{
	GENERATED_BODY()
public:
	static FVector GetGridSize();
	static UDigumWorldSwatchAsset* GetSwatchAsset(const FName& BlockID, const FName& ContentCategory);
	static FDigumWorldBlockTableRow* GetBlockTableRow(const FName& BlockID, UDataTable* InTable);
	static UDataTable* GetSwatchDataTable(const FName ContentCategory);
	static TSubclassOf<ADigumWorldActorChild> GetChildActorClass(const FName& BlockID, const FName& ContentCategory);
	static bool GetLocalSectionSize(const FName& InContentCategoryName, int32& OutWidth, int32& OutHeight);
	static bool GetSectionCoordinates(const FName& InContentCategoryName, const FVector& WorldLocation, int32& OutXCoordinate, int32& OutYCoordinate);
	static bool GetSectionCoordinates(const FName& InContentCategoryName, const TArray<FVector>& InWorldLocationArray, TArray<FDigumWorldProceduralSectionCoordinate>& OutSectionCoordinateArray);
	static void ConvertToSectionCoordinates(const FVector& InWorldLocation, const FVector2D& InSectionUnitSize, FDigumWorldProceduralSectionCoordinate& OutSectionCoordinate);
};
