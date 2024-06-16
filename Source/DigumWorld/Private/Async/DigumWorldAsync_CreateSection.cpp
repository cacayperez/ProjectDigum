// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldAsync_CreateSection.h"

#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMapLoader.h"

FDigumWorldAsync_CreateSection::FDigumWorldAsync_CreateSection(UDigumWorldMapAsyncLoader* InLoader, const FName& InSeed, const int32& InMapWidth,
                                                               const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InSectionWidth,
                                                               const int32& InSectionHeight, const int32& InNumberOfHierarchies,
                                                               const int32& InSectionCount_HorizontalAxis, const int32& InSectionCount_VerticalAxis,  const FDigumWorldProceduralDefinition& InProceduralDefinition)
{
	Loader = InLoader;
	Seed = InSeed;
	MapWidth = InMapWidth;
	MapHeight = InMapHeight;
	SectionX = InSectionX;
	SectionY = InSectionY;
	SectionWidth = InSectionWidth;
	SectionHeight = InSectionHeight;
	NumberOfHierarchies = InNumberOfHierarchies;
	ProceduralDefinition = InProceduralDefinition;
	SectionCount_HorizontalAxis = InSectionCount_HorizontalAxis;
	SectionCount_VerticalAxis = InSectionCount_VerticalAxis;
}

FDigumWorldAsync_CreateSection::~FDigumWorldAsync_CreateSection()
{
}

void FDigumWorldAsync_CreateSection::DoWork()
{
	
}
