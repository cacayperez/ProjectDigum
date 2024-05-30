// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldProceduralAsset.h"
#include "UObject/Object.h"
#include "DigumWorldGenerator.generated.h"


class UDigumWorldProceduralAsset;

USTRUCT()
struct FDigumWorldProceduralCoordinate
{
	GENERATED_BODY()
public:
	FDigumWorldProceduralCoordinate() : X(0), Y(0) { }
	FDigumWorldProceduralCoordinate(const int32& InX, const int32& InY) { X = InX; Y = InY; }
	FDigumWorldProceduralCoordinate(const int32& InX, const int32& InY, const float InNoiseValue) { X = InX; Y = InY; NoiseValue = InNoiseValue;}
	
	UPROPERTY()
	int32 X;

	UPROPERTY()
	int32 Y;
	
	UPROPERTY()
	float NoiseValue = -1.0f;

	UPROPERTY()
	FName BlockID = NAME_None;
	
};

USTRUCT()
struct FDigumWorldProceduralCoordinateArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FDigumWorldProceduralCoordinate> Coordinates;

	void AddCoordinate(const int32& InX, const int32& InY)
	{
		FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate(InX, InY);
		Coordinates.Add(Coordinate);
	}

	void AddCoordinate(const int32& InX, const int32& InY, const float InNoiseValue)
	{
		FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate(InX, InY, InNoiseValue);
		Coordinates.Add(Coordinate);
	}

	int32 CoordinateCount() const { return Coordinates.Num(); }

	FDigumWorldProceduralCoordinate* GetCoordinate(const int32& InIndex)
	{
		if(InIndex < 0 || InIndex >= Coordinates.Num()) return nullptr;
		return &Coordinates[InIndex];
	}
};

// TMap Wrapper for CoordinateArray
USTRUCT()
struct FDigumWorldProceduralMappedCoordinates
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<FName, FDigumWorldProceduralCoordinateArray> MappedCoordinates;

	TMap<FName, FDigumWorldProceduralCoordinateArray>* GetMappedCoordinates() { return &MappedCoordinates; }
	
	void AddCoordinate(const FName& InBlockID, const int32& InX, const int32& InY)
	{
		MappedCoordinates.FindOrAdd(InBlockID).AddCoordinate(InX, InY);
	}
};

USTRUCT()
struct FDigumWorldProceduralSection
{
	GENERATED_BODY()
public:
	FDigumWorldProceduralSection() : SectionCoordinateX(0), SectionCoordinateY(0) { }
	FDigumWorldProceduralSection(const int32 InX, const int32 InY) { SectionCoordinateX = InX; SectionCoordinateY = InY; }
	UPROPERTY()
	int32 SectionCoordinateX;

	UPROPERTY()
	int32 SectionCoordinateY;
	
	UPROPERTY()
	TMap<int32, FDigumWorldProceduralMappedCoordinates> CoordinateData;


	void AddCoordinate(const int32 Hierarchy, const FName& BlockID, const int32 InX, const int32 InY)
	{
		CoordinateData.FindOrAdd(Hierarchy).AddCoordinate(BlockID, InX, InY);
	}

	FDigumWorldProceduralMappedCoordinates* GetCoordinateArray(const int32&  Hierarchy)
	{
		return CoordinateData.Find(Hierarchy);
	}

	FDigumWorldProceduralMappedCoordinates* GetMappedCoordinates(const int32& InHierarchy)
	{
		return CoordinateData.Find(InHierarchy);
	}

	int32 GetX() const { return SectionCoordinateX; }
	int32 GetY() const { return SectionCoordinateY; }
};

USTRUCT()
struct FDigumWorldProceduralMap
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 Seed;

	UPROPERTY()
	TArray<FDigumWorldProceduralSection> Sections;

	void AddSection(const FDigumWorldProceduralSection& InSectionData)
	{
		Sections.Add(InSectionData);
	}

	int32 GetSectionCount() const { return Sections.Num(); }

	FDigumWorldProceduralSection* GetSection(const int32& InX, const int32& InY)
	{
		for(FDigumWorldProceduralSection& Section : Sections)
		{
			if(Section.SectionCoordinateX == InX && Section.SectionCoordinateY == InY)
			{
				return &Section;
			}
		}
		return nullptr;
	}

	TArray<FDigumWorldProceduralSection>& GetSections() { return Sections; }
};


USTRUCT()
struct FDigumWorldProceduralRules
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 Seed = 0;
	
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
	int32 GroundZero = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxElevation = 100;

	UPROPERTY()
	int32 NumberOfHierarchies = 3;

	UPROPERTY(EditAnywhere) 
	TSoftObjectPtr<UDigumWorldProceduralAsset> ProceduralAsset;

	UDigumWorldProceduralAsset* GetProceduralAsset() const { return UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);}
};

/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldGenerator : public UObject
{
	GENERATED_BODY()
private:
	static void GenerateUndergroundVeins(FDigumWorldProceduralMap& OutMap);
	static void GenerateTrees(FDigumWorldProceduralMap& OutMap);
	static void GenerateFoliage(FDigumWorldProceduralMap& OutMap);
	static float GetPerlinNoiseValue(const int32 InX, const int32 InY, const FRandomStream& RandomStream);
	static float NormalizeNoiseValue(const float InNoiseValue);
	static FName GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>> OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks);
	static bool GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights);
	// static void GenerateSection(const )
public:
	static bool GenerateSection(const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream, const TArray<FDigumWorldProceduralBlock>& InBlocks, const TArray<TPair<float, float>>& InCumulativeWeights,
	                          FDigumWorldProceduralSection& OutSection, const int32& NumOfHierarchies = 1);
	static void GenerateWorldMap(const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralMap& OutMap);

	

};
	