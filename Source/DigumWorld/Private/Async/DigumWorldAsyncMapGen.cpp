// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldAsyncMapGen.h"
#include "Asset/DigumWorldProceduralAsset.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"
#include "Procedural/DigumWorldMapLoader.h"


FDigumWorldAsyncMapGen::FDigumWorldAsyncMapGen(UDigumWorldMapAsyncLoader* InLoader, UDigumWorldProceduralAsset* InProceduralAsset, const FName& InSeed, const FVector& InGridSize,
                                               const int32& InSectionWidth, const int32& InSectionHeight, const int32& InSectionCount_HorizontalAxis,
                                               const int32& InSectionCount_VerticalAxis, const int32& InNumberOfHierarchies)
{
	Loader = InLoader;
	ProceduralAsset = InProceduralAsset;
	Seed = InSeed;
	GridSize = InGridSize;
	SectionWidth = InSectionWidth;
	SectionHeight = InSectionHeight;
	SectionCount_HorizontalAxis = InSectionCount_HorizontalAxis;
	SectionCount_VerticalAxis = InSectionCount_VerticalAxis;
	NumberOfHierarchies = InNumberOfHierarchies;
	
}

FDigumWorldAsyncMapGen::~FDigumWorldAsyncMapGen()
{
}

void FDigumWorldAsyncMapGen::DoWork()
{
	if(!Loader || !ProceduralAsset)
	{
		return;
	}
	
	

}
