// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldProceduralAsset.h"
#include "Procedural/DigumWorldMap.h"

struct FDigumWorldProceduralSection;
class UDigumWorldMapSectionComponent;
/**
 * 
 */
class DIGUMWORLD_API FDigumWorldAsyncSectionLoader : public FNonAbandonableTask
{
public:
	FDigumWorldAsyncSectionLoader(UDigumWorldMapSectionComponent* InComponent, FDigumWorldMap InMap, FDigumWorldProceduralDefinition InProceduralDefinition, const int32& InX, const int32& InY);
	~FDigumWorldAsyncSectionLoader();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(DigumWorldAsyncSectionLoader, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

protected:
	UDigumWorldMapSectionComponent* Component;
	FDigumWorldProceduralDefinition ProceduralDefinition;
	FDigumWorldMap Map;
	int32 X;
	int32 Y;
};
