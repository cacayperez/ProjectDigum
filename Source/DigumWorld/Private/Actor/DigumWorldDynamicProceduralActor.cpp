// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldDynamicProceduralActor.h"

#include "Procedural/DigumWorldGenerator.h"


// Sets default values
ADigumWorldDynamicProceduralActor::ADigumWorldDynamicProceduralActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADigumWorldDynamicProceduralActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADigumWorldDynamicProceduralActor::InitializeDefaultSections()
{
	
}

TArray<FDigumWorldProceduralSectionCoordinate> ADigumWorldDynamicProceduralActor::GetSectionCoordinatesInRadius(
	const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& Radius, const int32& XMin,
	const int32& XMax, const int32& YMin, const int32 YMax) const
{
	TArray<FDigumWorldProceduralSectionCoordinate> SectionCoordinates;

	const int32 StartX = InStartCoordinate.X - Radius;
	const int32 StartY = InStartCoordinate.Y - Radius;
	const int32 EndX = InStartCoordinate.X + Radius;
	const int32 EndY = InStartCoordinate.Y + Radius;
	
	for(int32 x = StartX; x <= EndX; x++)
	{
		for(int32 y = StartY; y <= EndY; y++)
		{
			if(x >= XMin && x <= XMax && y >= YMin && y <= YMax)
			{
				const int32 dx = InStartCoordinate.X - x;
				const int32 dy = InStartCoordinate.Y - y;
				SectionCoordinates.Add(FDigumWorldProceduralSectionCoordinate(x, y));
			}
		}
	}
	
	return SectionCoordinates;
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize)
{
	FDigumWorldProceduralSectionCoordinate StartCoordinate = GetSectionCoordinate(InWorldLocation);

	const int32 MaxX = GetMap()->SectionCount_HorizontalAxis - 1;
	const int32 MaxY = GetMap()->SectionCount_VerticalAxis - 1;
	TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRadius(StartCoordinate, HalfSize, 0, MaxX, 0, MaxY);

	for(const FDigumWorldProceduralSectionCoordinate& Coordinate : Coordinates)
	{
		FDigumWorldProceduralSection Section;
		if(GetSection(Coordinate.X, Coordinate.Y, Section))
		{
			CreateSection(Section);
		}
	}
}



// Called every frame
void ADigumWorldDynamicProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

