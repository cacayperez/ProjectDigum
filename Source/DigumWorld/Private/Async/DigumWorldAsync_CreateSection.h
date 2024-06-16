// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldProceduralAsset.h"

class UDigumWorldMapAsyncLoader;
struct FDigumWorldProceduralDefinition;
/**
 * 
 */
class DIGUMWORLD_API FDigumWorldAsync_CreateSection : public FNonAbandonableTask
{
public:
	FDigumWorldAsync_CreateSection(UDigumWorldMapAsyncLoader* InLoader, const FName& InSeed, const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InSectionWidth, const int32& InSectionHeight, const int32& InNumberOfHierarchies, const int32& InSectionCount_HorizontalAxis, const int32& InSectionCount_VerticalAxis, const FDigumWorldProceduralDefinition& InProceduralDefinition);
	~FDigumWorldAsync_CreateSection();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDigumWorldAsync_CreateSection, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
private:
	UDigumWorldMapAsyncLoader* Loader;
	FName Seed;
	int32 MapWidth;
	int32 MapHeight;
	int32 SectionX;
	int32 SectionY;
	int32 SectionWidth;
	int32 SectionHeight;
	int32 NumberOfHierarchies;
	FDigumWorldProceduralDefinition ProceduralDefinition;
	int32 SectionCount_HorizontalAxis;
	int32 SectionCount_VerticalAxis;
};
