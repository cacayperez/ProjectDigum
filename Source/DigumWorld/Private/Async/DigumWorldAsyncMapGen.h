// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Procedural/DigumWorldGenerator.h"

class UDigumWorldMapAsyncLoader;
class UDigumWorldProceduralAsset;
/**
 * 
 */
class DIGUMWORLD_API FDigumWorldAsyncMapGen : public FNonAbandonableTask
{
public:
	FDigumWorldAsyncMapGen(UDigumWorldMapAsyncLoader* InLoader, UDigumWorldProceduralAsset* InProceduralAsset, const FName& InSeed,const FVector& InGridSize, const int32& InSectionWidth, const int32& InSectionHeight, const int32& InSectionCount_HorizontalAxis, const int32& InSectionCount_VerticalAxis, const int32& InNumberOfHierarchies);
	~FDigumWorldAsyncMapGen();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDigumWorldAsyncMapGen, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
protected:
	UDigumWorldMapAsyncLoader* Loader;
	UDigumWorldProceduralAsset* ProceduralAsset;
	FName Seed;
	FVector GridSize;
	int32 SectionWidth;
	int32 SectionHeight;
	int32 SectionCount_HorizontalAxis;
	int32 SectionCount_VerticalAxis;
	int32 NumberOfHierarchies;

	// bool GetSection(const int32& X, const int32& Y, FDigumWorldProceduralSection& OutSection);

};
