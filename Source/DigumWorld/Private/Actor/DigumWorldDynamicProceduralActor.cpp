// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldDynamicProceduralActor.h"

#include "Actor/DigumWorldActorSection.h"
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

TArray<FDigumWorldProceduralSectionCoordinate> ADigumWorldDynamicProceduralActor::GetSectionCoordinatesInRect(
	const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin,
	const int32& XMax, const int32& YMin, const int32 YMax) const
{
	TArray<FDigumWorldProceduralSectionCoordinate> SectionCoordinates;

	const int32 StartX = InStartCoordinate.X - HalfSize;
	const int32 StartY = InStartCoordinate.Y - HalfSize;
	const int32 EndX = InStartCoordinate.X + HalfSize;
	const int32 EndY = InStartCoordinate.Y + HalfSize;
	
	for(int32 x = StartX; x <= EndX; x++)
	{
		for(int32 y = StartY; y <= EndY; y++)
		{
			if(x >= XMin && x <= XMax && y >= YMin && y <= YMax)
			{
				SectionCoordinates.Add(FDigumWorldProceduralSectionCoordinate(x, y));
			}
		}
	}
	
	return SectionCoordinates;
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize)
{
	FDigumWorldProceduralSectionCoordinate StartCoordinate = GetSectionCoordinate(InWorldLocation);

	const int32 MaxX = GetMap()->SectionCount_HorizontalAxis -1;
	const int32 MaxY = GetMap()->SectionCount_VerticalAxis -1;
	const TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRect(StartCoordinate, HalfSize, 0, MaxX, 0, MaxY);
	ActiveCoordinates = Coordinates;

	UE_LOG(LogTemp, Warning, TEXT("=== Sections Array: %d"), SectionDataArray.Num());
	
	for(const FDigumWorldProceduralSectionCoordinate& Coordinate : ActiveCoordinates)
	{
		// FDigumWorldProceduralSection Section;
		if(GetSectionActor(Coordinate.X, Coordinate.Y)) continue;
		FDigumWorldProceduralSection Section;
		for(auto& SectionItem: SectionDataArray)
		{
			if(SectionItem.GetX() == Coordinate.X && SectionItem.GetY() == Coordinate.Y )
			{
				Section = SectionItem;
			}
		}
		CreateSection(Section);
		//
		// UE_LOG(LogTemp, Warning, TEXT("=== Index: %d, %i, %i"), Index, Coordinate.X, Coordinate.Y);
		/*if(Section)
		{
			
		}*/
		/*if(GetSection(Coordinate.X, Coordinate.Y, Section))
		{
			CreateSection(Section);
		}*/
	}

	PurgeChunks();
}

void ADigumWorldDynamicProceduralActor::PurgeChunks()
{
	TArray<ADigumWorldActorSection*> Array;
	for(auto Coordinate : ActiveCoordinates)
	{
		if(ADigumWorldActorSection* Actor = GetSectionActor(Coordinate.X, Coordinate.Y))
		{
			Array.Add(Actor);
		}
	}

	for(auto Actor : SectionActors)
	{
		if(Actor && !Array.Contains(Actor))
		{
			Actor->DestroySection();
		}
	}
}


// Called every frame
void ADigumWorldDynamicProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

