// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldProceduralAsset.h"

struct FDigumWorldMap;
class UDigumWorldMapLoaderComponent;
/**
 * 
 */
class DIGUMWORLD_API FDigumWorldAsyncSection : public FNonAbandonableTask
{
public:
	FDigumWorldAsyncSection(UDigumWorldMapLoaderComponent* InComponent, const FDigumWorldMap* InMap, const FDigumWorldProceduralDefinition* InProceduralDefinition, const int32& InX, const int32& InY);
	~FDigumWorldAsyncSection();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDigumWorldAsyncSection, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

protected:
	UDigumWorldMapLoaderComponent* Component;
	const FDigumWorldProceduralDefinition* ProceduralDefinition;
	const FDigumWorldMap* Map;
	int32 X;
	int32 Y;
};
