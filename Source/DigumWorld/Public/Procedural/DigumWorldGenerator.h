// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldProceduralAsset.h"
#include "UObject/Object.h"
#include "DigumWorldGenerator.generated.h"


struct FDigumWorldMap;
class UDigumWorldProceduralAsset;

USTRUCT()
struct FDigumWorldProceduralSectionCoordinate
{
	GENERATED_BODY()
public:
	FDigumWorldProceduralSectionCoordinate() : X(-1), Y(-1) { }
	FDigumWorldProceduralSectionCoordinate(const int32 InX, const int32 InY) { X = InX; Y = InY; }
	UPROPERTY(VisibleAnywhere)
	int32 X;

	UPROPERTY(VisibleAnywhere)
	int32 Y;

	bool operator==(const FDigumWorldProceduralSectionCoordinate& InCoordinate) const
	{
		return X == InCoordinate.X && Y == InCoordinate.Y;
	}

	bool IsValid() const { return X >= 0 && Y >= 0; }
	FString ToString() const
	{
		return FString::Printf(TEXT("X: %d, Y: %d"), X, Y);
	}
};

USTRUCT()
struct FDigumWorldProceduralCoordinate
{
	GENERATED_BODY()
public:
	FDigumWorldProceduralCoordinate() : X(0), Y(0), GlobalX(0), GlobalY(0) { }
	FDigumWorldProceduralCoordinate(const int32& InX, const int32& InY) { X = InX; Y = InY; }
	FDigumWorldProceduralCoordinate(const int32& InX, const int32& InY, const int32& InGlobalX, const int32& InGlobalY) { X = InX; Y = InY;  GlobalX = InGlobalX; GlobalY = InGlobalY; }
	FDigumWorldProceduralCoordinate(const FName& InBlockID, const int32& InX, const int32& InY, const float InNoiseValue) { BlockID = InBlockID; X = InX; Y = InY; NoiseValue = InNoiseValue;}
	
	UPROPERTY()
	int32 X = -1;

	UPROPERTY()
	int32 Y = -1;

	UPROPERTY()
	int32 GlobalX = -1;

	UPROPERTY()
	int32 GlobalY = -1;

	UPROPERTY()
	int32 Hierarchy = 0;
	
	UPROPERTY()
	float NoiseValue = -1.0f;

	UPROPERTY()
	FName BlockID = NAME_None;

	UPROPERTY()
	bool bIsDirectSurfaceBlock = false;

	UPROPERTY()
	bool bBlocksPlacement = true;

	bool IsDirectSurfaceBlock() const { return bIsDirectSurfaceBlock; }

	UPROPERTY()
	bool bHasTopNeighbor = false;

	UPROPERTY()
	bool bHasLeftNeighbor = false;

	UPROPERTY()
	bool bHasRightNeighbor = false;

	UPROPERTY()
	bool bHasBottomNeighbor = false;
};

USTRUCT()
struct FDigumWorldProceduralCoordinateArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FDigumWorldProceduralCoordinate> Coordinates;

	/*void AddCoordinate(const int32& InX, const int32& InY)
	{
		FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate(InX, InY);
		Coordinates.Add(Coordinate);
	}*/

	void AddCoordinate(const FName& InBlockID, const int32& InLocalX, const int32& InLocalY, const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchy, const float& InNoiseValue)
	{
		FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate(InLocalX, InLocalY, InGlobalX, InGlobalY);
		Coordinate.BlockID = InBlockID;
		Coordinate.Hierarchy = InHierarchy;
		Coordinate.NoiseValue = InNoiseValue;
		Coordinates.Add(Coordinate);
	}

	void AddCoordinate(const FDigumWorldProceduralCoordinate& InCoordinate)
	{
		Coordinates.Add(InCoordinate);
	}

	int32 CoordinateCount() const { return Coordinates.Num(); }

	FDigumWorldProceduralCoordinate* GetCoordinate(const int32& InIndex)
	{
		if(InIndex < 0 || InIndex >= Coordinates.Num()) return nullptr;
		return &Coordinates[InIndex];
	}

	FDigumWorldProceduralCoordinate* GetCoordinate(const int32& InX, const int32& InY, const int32& InHierarchy)
	{
		for(FDigumWorldProceduralCoordinate& Coordinate : Coordinates)
		{
			if(Coordinate.X == InX && Coordinate.Y == InY && Coordinate.Hierarchy == InHierarchy)
			{
				return &Coordinate;
			}
		}
		return nullptr;
	}

	void MakeMappedCoordinates(TMap<FName, FDigumWorldProceduralCoordinateArray>& OutMappedCoordinates)
	{
		for(FDigumWorldProceduralCoordinate& Coordinate : Coordinates)
		{
			OutMappedCoordinates.FindOrAdd(Coordinate.BlockID).AddCoordinate(Coordinate);
		}
	}

	void Append(const FDigumWorldProceduralCoordinateArray* CoordinateArray)
	{
		Coordinates.Append(CoordinateArray->Coordinates);
	}

	const TArray<FDigumWorldProceduralCoordinate>& GetCoordinates() const { return Coordinates; }
};

// TMap Wrapper for CoordinateArray
/*USTRUCT()
struct FDigumWorldProceduralMappedCoordinates
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<FName, FDigumWorldProceduralCoordinateArray> MappedCoordinates;

	TMap<FName, FDigumWorldProceduralCoordinateArray>* GetMappedCoordinates() { return &MappedCoordinates; }
	
	/*void AddCoordinate(const FName& InBlockID, const int32& InX, const int32& InY)
	{
		MappedCoordinates.FindOrAdd(InBlockID).AddCoordinate(InX, InY);
	}#1#
};*/

USTRUCT()
struct FDigumWorldProceduralSection
{
	GENERATED_BODY()
public:
	FDigumWorldProceduralSection() : SectionCoordinate(0, 0) { }
	FDigumWorldProceduralSection(const int32 InX, const int32 InY) { SectionCoordinate.X = InX; SectionCoordinate.Y = InY; }

	UPROPERTY(VisibleAnywhere)
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	
	UPROPERTY()
	FDigumWorldProceduralCoordinateArray CoordinateArrayData;


	void AddCoordinate(const FName& InBlockID, const int32& InLocalX, const int32& InLocalY, const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchy, const float& InNoiseValue)
	{
		CoordinateArrayData.AddCoordinate(InBlockID, InLocalX, InLocalY, InGlobalX, InGlobalY, InHierarchy, InNoiseValue);
	}
	
	void AddCoordinate(FDigumWorldProceduralCoordinate& InCoordinate)
	{
		CoordinateArrayData.AddCoordinate(InCoordinate);
	}
	
	FDigumWorldProceduralCoordinateArray* GetCoordinateArray()
	{
		return &CoordinateArrayData;
	}

	int32 GetX() const { return SectionCoordinate.X; }
	int32 GetY() const { return SectionCoordinate.Y; }
	
	FDigumWorldProceduralSectionCoordinate GetSectionCoordinate() const { return SectionCoordinate; }
	bool IsInitialized() const { return SectionCoordinate.X != -1 && SectionCoordinate.Y != -1; }
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
			if(Section.SectionCoordinate.X == InX && Section.SectionCoordinate.Y == InY)
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

	UPROPERTY(EditAnywhere)
	int32 NumberOfHierarchies = 3;

	UPROPERTY(EditAnywhere) 
	TSoftObjectPtr<UDigumWorldProceduralAsset> ProceduralAsset;

	UDigumWorldProceduralAsset* GetProceduralAsset() const { return UDigumAssetManager::GetAsset<UDigumWorldProceduralAsset>(ProceduralAsset);}

};

USTRUCT()
struct FDigumWorldProceduralPlacedBlocks
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FDigumWorldProceduralBlock Block;

	UPROPERTY()
	int32 OriginX;

	UPROPERTY()
	int32 OriginY;
};

/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldGenerator : public UObject
{
	GENERATED_BODY()
private:
	static float GetPerlinNoiseValue1D(const float InX, const FRandomStream& InRandomStream);
	static float GetPerlinNoiseValue2D(const float InX, const int32 InY, const FRandomStream& InRandomStream);
	static float GetPerlinNoiseValue3D(const float InX, const int32 InY, const int32 InZ, const FRandomStream& InRandomStream);
	
	static void GenerateUndergroundVeins(FDigumWorldProceduralMap& OutMap);
	static void GenerateTrees(FDigumWorldProceduralMap& OutMap);
	static void GenerateFoliage(FDigumWorldProceduralMap& OutMap);
	static float GetGroundNoise(const float& X, const FRandomStream& InRandomStream);
	static float QuadraticBezier(float t, float P0, float P1, float P2);
	static float CubicBezier(float t, float P0, float P1, float P2, float P3);
	static TArray<float> SmoothTerrain(const TArray<float>& GroundCurve, int32 SmoothingPasses);
	
	static float NormalizeNoiseValue(const float InNoiseValue);
	static FName GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>> OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks);
	static FName GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>> OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock_Sized>& Blocks);
	static bool GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights);
	static bool GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock_Sized>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights);
	static TArray<float> GenerateGroundCurve(const int32& InWidth, const int32& InHeight, const int32& SectionX, const FRandomStream& InRandomStream);
	static bool IsSurfacePoint(const int32& PositionX, const int32& PositionY, const TArray<float>& GroundCurve, const int32& InWidth, const int32& InSectionX);
	// static void GenerateSection(const )
public:
	static bool GenerateSection(const FDigumWorldMap &InMap, const int32& InSectionX, const int32& InSectionY, const UDigumWorldProceduralAsset* ProceduralAsset, FDigumWorldProceduralSection& OutSection);
	static bool GenerateSection(const int32& InSeed, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralSection& OutSection);
	static bool GenerateSection(const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream, 
	                           const int32& NumOfHierarchies,  const UDigumWorldProceduralAsset* ProceduralAsset, FDigumWorldProceduralSection& OutSection);
	static bool GenerateFoliage(const FName& InSeedName, TArray<FDigumWorldProceduralSection>& InSectionArray, const UDigumWorldProceduralAsset* ProceduralAsset);
	static void GenerateWorldMap(const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralMap& OutMap);
	
	static bool GetCumulativeWeights(TArray<TPair<float, float>>& OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks)
	{
		if(Blocks.IsEmpty()) return false;
		
		float TotalWeight = 0.0f;
		float CumulativeSum = 0.0f;
		for(const auto&	[BlockID, Weight] : Blocks)
		{
			// Weight Total
			TotalWeight += Weight;
		}
	
		for(const auto&	[BlockID, Weight] : Blocks)
		{
			float StartRange = CumulativeSum;
			CumulativeSum += Weight;
			float EndRange = CumulativeSum;
			OutCumulativeWeights.Add(TPair<float, float>(StartRange / TotalWeight, EndRange / TotalWeight));
		}

		return true;
	}
};
	