// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldMapLoader.generated.h"
UENUM()
enum EDigumWorldMapGenState : uint8
{
	GenState_None,
	GenState_Initializing,
	GenState_GeneratingTerrain,
	// TODO Add other generation states
	/*GenState_GeneratingCaves,
	GenState_GeneratingWater,
	GenStage_GeneratingStructures,*/
	GenState_GeneratingFoliage,
	GenState_GeneratingTrees,
	GenState_Completed
};

UCLASS()
class DIGUMWORLD_API UDigumWorldMapAsyncLoader : public UObject
{
	GENERATED_BODY()

public:
	UDigumWorldMapAsyncLoader();
	

	const TCHAR* GetGenStateAsString(const EDigumWorldMapGenState& InState)
	{
		switch(InState)
		{
		case EDigumWorldMapGenState::GenState_None:
			return TEXT("None");
		case EDigumWorldMapGenState::GenState_Initializing:
			return TEXT("Initializing");
		case EDigumWorldMapGenState::GenState_GeneratingTerrain:
			return TEXT("Generating Terrain");
		case EDigumWorldMapGenState::GenState_GeneratingFoliage:
			return TEXT("Generating Foliage");
		case EDigumWorldMapGenState::GenState_GeneratingTrees:
			return TEXT("Generating Trees");
		case EDigumWorldMapGenState::GenState_Completed:
			return TEXT("Completed");
		default:
			return TEXT("Unknown");
		}
	}
};