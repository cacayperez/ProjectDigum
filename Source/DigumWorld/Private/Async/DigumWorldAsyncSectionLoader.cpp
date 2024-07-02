// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldAsyncSectionLoader.h"

#include "Components/DigumWorldMapSectionComponent.h"
#include "Procedural/DigumWorldGenerator.h"


FDigumWorldAsyncSectionLoader::FDigumWorldAsyncSectionLoader(UDigumWorldMapSectionComponent* InComponent,
                                                           FDigumWorldMap InMap, FDigumWorldProceduralDefinition InProceduralDefinition, const int32& InX,
                                                           const int32& InY)
{
	Component = InComponent;
	Map = InMap;
	ProceduralDefinition = InProceduralDefinition;
	X = InX;
	Y = InY;
}

void FDigumWorldAsyncSectionLoader::DoWork()
{
	UE_LOG(LogTemp, Warning, TEXT("Working: %i, %i"), X, Y);
	if(Component)
	{
		FDigumWorldProceduralSection OutSection;
		if(UDigumWorldGenerator::CreateSection(Map, X, Y,  ProceduralDefinition, OutSection))
		{
			UE_LOG(LogTemp, Warning, TEXT("Working: Result Generated %i, %i"), OutSection.GetX(), OutSection.GetY());
			const TSharedPtr<FDigumWorldProceduralSection> Shared = MakeShared<FDigumWorldProceduralSection>(OutSection);
			Component->SectionQueue.Enqueue(Shared);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Add Section to Queue"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Component is NULL"));
	}
}

FDigumWorldAsyncSectionLoader::~FDigumWorldAsyncSectionLoader()
{
}
