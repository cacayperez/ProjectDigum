// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldMap.generated.h"



USTRUCT()
struct FDigumWorldMap
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	bool bIsInitialized = false;
	
public:
	FDigumWorldMap()
	{
		SectionWidth = 12;
		SectionHeight = 12;
		GridSize = FVector(60.0f, 90.0f, 60.0f);
		SectionCount_HorizontalAxis = 1;
		SectionCount_VerticalAxis = 1;
		NumberOfHierarchies = 3;
		bIsInitialized = true;
	}

	FDigumWorldMap(const FName InSeed, const FVector InGridSize, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies)
	{
		Seed = InSeed;
		GridSize = InGridSize;
		SectionWidth = InSectionWidth;
		SectionHeight = InSectionHeight;
		SectionCount_HorizontalAxis = InSectionCount_HorizontalAxis;
		SectionCount_VerticalAxis = InSectionCount_VerticalAxis;
		NumberOfHierarchies = InNumberOfHierarchies;
		bIsInitialized = true;
	}

	FDigumWorldMap(const FDigumWorldProceduralRules& ProceduralRules, const FVector InGridSize)
	{
		Seed = ProceduralRules.Seed;
		GridSize = InGridSize;
		SectionWidth = ProceduralRules.SectionWidth;
		SectionHeight = ProceduralRules.SectionHeight;
		SectionCount_HorizontalAxis = ProceduralRules.SectionCount_HorizontalAxis;
		SectionCount_VerticalAxis = ProceduralRules.SectionCount_VerticalAxis;
		NumberOfHierarchies = ProceduralRules.NumberOfHierarchies;
		bIsInitialized = true;
		
	}

	UPROPERTY()
	FName Seed = TEXT("Hello World!");

	UPROPERTY(EditAnywhere)
	int32 SectionWidth = 12;

	UPROPERTY(EditAnywhere)
	int32 SectionHeight = 12;

	// Spans both negative and positive values
	UPROPERTY(EditAnywhere)
	int32 SectionCount_HorizontalAxis = 1;

	// Spans both negative and positive values
	UPROPERTY(EditAnywhere)
	int32 SectionCount_VerticalAxis = 1;
	
	UPROPERTY(EditAnywhere)
	int32 NumberOfHierarchies = 3;
	
	UPROPERTY()
	FVector GridSize = FVector::ZeroVector;

	UPROPERTY()
	TArray<FDigumWorldProceduralSection> SectionDataArray;

	FVector2D GetSectionUnitSize() const
	{
		return FVector2D(SectionWidth * GridSize.X, SectionHeight * GridSize.Z);
	}

	int32 GetTotalSectionWidth() const
	{
		return SectionWidth * SectionCount_HorizontalAxis;
	}

	int32 GetTotalSectionHeight() const
	{
		return SectionHeight * SectionCount_VerticalAxis;
	}

	FVector GetWorldOffset() const
	{
		const FVector2D MapUnitSize = GetMapUnitSize();
		return FVector(-MapUnitSize.X / 2, 0.0f, MapUnitSize.Y / 2);
	}
	FVector2D GetMapUnitSize() const
	{
		const FVector2D SectionUnitSize = GetSectionUnitSize();
		return FVector2D(SectionUnitSize.X * SectionCount_HorizontalAxis, SectionUnitSize.Y * SectionCount_VerticalAxis);
	}
	int32 GetLocalSectionWidth() const { return SectionWidth; }
	int32 GetLocalSectionHeight() const { return SectionHeight; }
	
	bool IsInitialized() const { return bIsInitialized; }
	void AddSection(const FDigumWorldProceduralSection& InSection)
	{
		SectionDataArray.Add(InSection);
	}
};


