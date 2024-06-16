// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldAsyncSection.h"

#include "Components/DigumWorldMapLoaderComponent.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"



FDigumWorldAsyncSection::FDigumWorldAsyncSection(UDigumWorldMapLoaderComponent* InComponent,
	const FDigumWorldMap* InMap, const FDigumWorldProceduralDefinition* InProceduralDefinition, const int32& InX,
	const int32& InY)
{
	Component = InComponent;
	Map = InMap;
	ProceduralDefinition = InProceduralDefinition;
	X = InX;
	Y = InY;
}

FDigumWorldAsyncSection::~FDigumWorldAsyncSection()
{
}

void FDigumWorldAsyncSection::DoWork()
{
	if(Component && Map)
	{
		FDigumWorldProceduralSection OutSection;
		if(UDigumWorldGenerator::GenerateSection(*Map, X, Y,  *ProceduralDefinition, OutSection))
		{
			UE_LOG(LogTemp, Warning, TEXT("Working: Section Generated %i, %i"), X, Y);
			UE_LOG(LogTemp, Warning, TEXT("Working: Result Generated %i, %i"), OutSection.GetX(), OutSection.GetY());
			const TSharedPtr<FDigumWorldProceduralSection> Shared = MakeShared<FDigumWorldProceduralSection>(OutSection);
			Component->SectionQueue.Enqueue(Shared);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Add Section to Queue"));
		}
	}
}
