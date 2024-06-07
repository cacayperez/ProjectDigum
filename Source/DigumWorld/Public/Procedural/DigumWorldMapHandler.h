// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumWorldMap.h"
#include "UObject/Object.h"
#include "DigumWorldMapHandler.generated.h"

class UDigumWorldProceduralAsset;
struct FDigumWorldProceduralSection;
/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldMapHandler : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	FDigumWorldMap Map;

	UPROPERTY()
	UDigumWorldProceduralAsset* ProceduralAsset;

public:
	UDigumWorldMapHandler();

	void SetProceduralAsset(UDigumWorldProceduralAsset* InProceduralAsset) { ProceduralAsset = InProceduralAsset; }
	void GenerateMap(const FName InSeed, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies);
	bool GetSection(const int32& InSectionX, const int32& InSectionY, FDigumWorldProceduralSection& OutSection) const;

	FDigumWorldMap GetMap() const { return Map; }
	UDigumWorldProceduralAsset* GetProceduralAsset() const;
};
