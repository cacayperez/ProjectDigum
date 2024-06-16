// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Procedural/DigumWorldMapHandler.h"

#include "Procedural/DigumWorldGenerator.h"

UDigumWorldMapHandler::UDigumWorldMapHandler()
{
}

void UDigumWorldMapHandler::GenerateMap(const FName InSeed, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies)
{
	//Map = FDigumWorldMap(InSeed, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis, InSectionCount_VerticalAxis, InNumberOfHierarchies);
}

bool UDigumWorldMapHandler::GetSection(const int32& InSectionX, const int32& InSectionY,
	FDigumWorldProceduralSection& OutSection) const 
{
	if(ProceduralAsset == nullptr) return false;

	return UDigumWorldGenerator::GenerateSection(Map, InSectionX, InSectionY, ProceduralAsset->ProceduralDefinition, OutSection);
}

UDigumWorldProceduralAsset* UDigumWorldMapHandler::GetProceduralAsset() const
{
	return ProceduralAsset;
}
