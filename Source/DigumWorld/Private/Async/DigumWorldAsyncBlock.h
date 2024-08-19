// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Subsystem/DigumWorldSubsystem.h"

class ADigumWorldActorChild;
/**
 * 
 */
class DIGUMWORLD_API FDigumWorldAsyncBlock : public FNonAbandonableTask
{
public:
	FDigumWorldAsyncBlock(ADigumWorldActorChild* InActorChild, const FName& InBlockID, const FVector& InGridSize, const FVector& InPositionOffset, const FDigumWorldProceduralCoordinateArray& InCoordinates, const FDigumWorldRequestParams& InParams);
	~FDigumWorldAsyncBlock();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDigumWorldAsyncBlock, STATGROUP_ThreadPoolAsyncTasks);
	}
	
	void DoWork();
private:
	ADigumWorldActorChild* ActorChild;
	FName BlockID;
	FVector GridSize;
	FVector PositionOffset;
	FDigumWorldRequestParams Params;
	FDigumWorldProceduralCoordinateArray Coordinates;
};
