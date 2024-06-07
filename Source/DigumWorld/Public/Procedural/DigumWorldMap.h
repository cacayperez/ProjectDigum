// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldMap.generated.h"

USTRUCT()
struct FDigumWorldMap
{
	GENERATED_BODY()
	UPROPERTY()
	bool bIsInitialized = false;
	
public:
	FDigumWorldMap()
	{
		SectionWidth = 12;
		SectionHeight = 12;
		SectionCount_HorizontalAxis = 1;
		SectionCount_VerticalAxis = 1;
		NumberOfHierarchies = 3;
		SectionUnitSize = FVector2D(SectionWidth * SectionCount_HorizontalAxis, SectionHeight * SectionCount_VerticalAxis);
		WorldOffset = FVector2D(-SectionUnitSize.X / 2, SectionUnitSize.Y / 2);
		bIsInitialized = true;
	}

	FDigumWorldMap(const FName InSeed, const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies)
	{
		Seed = InSeed;
		SectionWidth = InSectionWidth;
		SectionHeight = InSectionHeight;
		SectionCount_HorizontalAxis = InSectionCount_HorizontalAxis;
		SectionCount_VerticalAxis = InSectionCount_VerticalAxis;
		NumberOfHierarchies = InNumberOfHierarchies;
		SectionUnitSize = FVector2D(SectionWidth * SectionCount_HorizontalAxis, SectionHeight * SectionCount_VerticalAxis);
		WorldOffset = FVector2D(-SectionUnitSize.X / 2, SectionUnitSize.Y / 2);
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
	FVector2D SectionUnitSize = FVector2D::ZeroVector;

	UPROPERTY()
	FVector2D WorldOffset = FVector2D::ZeroVector;

	FVector2D GetSectionUnitSize() const
	{
		return SectionUnitSize;
	}

	int32 GetTotalSectionWidth() const
	{
		return SectionWidth * SectionCount_HorizontalAxis;
	}

	int32 GetTotalSectionHeight() const
	{
		return SectionHeight * SectionCount_VerticalAxis;
	}

	FVector2D GetWorldOffset() const
	{
		return WorldOffset;
	}

	int32 GetLocalSectionWidth() const { return SectionWidth; }
	int32 GetLocalSectionHeight() const { return SectionHeight; }

	bool IsInitialized() const { return bIsInitialized; }
};

