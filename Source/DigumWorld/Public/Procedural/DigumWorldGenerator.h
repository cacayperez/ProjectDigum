
#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldProceduralAsset.h"
#include "UObject/Object.h"
#include "DigumWorldGenerator.generated.h"


struct FDigumWorldMap;
class UDigumWorldProceduralAsset;


USTRUCT()
struct FDigumWorldBlockID
{
	GENERATED_BODY()
public:
	FDigumWorldBlockID() : BlockID(NAME_None) { }
	FDigumWorldBlockID(const FName& InBlockID, const int32 InVariant) { BlockID = InBlockID; Variant = InVariant;}

	UPROPERTY(VisibleAnywhere)
	FName BlockID;

	UPROPERTY(VisibleAnywhere)
	int32 Variant = 0;

	UPROPERTY(EditAnywhere)
	bool bIsBlocking = true;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDigumWorldSurfaceAttachType> AttachmentType = EDigumWorldSurfaceAttachType::DigumWorldAttach_Default;
};


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
	TArray<FDigumWorldBlockID> BlockIDs;

	UPROPERTY()
	bool bIsDirectSurfaceBlock = false;

	UPROPERTY()
	bool bBlocksPlacement = true;

	

	UPROPERTY()
	bool bHasTopNeighbor = false;

	UPROPERTY()
	bool bHasLeftNeighbor = false;

	UPROPERTY()
	bool bHasRightNeighbor = false;

	UPROPERTY()
	bool bHasBottomNeighbor = false;

	int32 GetVariant(const FName& BlockIDName) const
	{
		for(const auto& Item : BlockIDs)
		{
			if(Item.BlockID == BlockIDName)
			{
				return Item.Variant;
			}
		}
		return 0;
	}

	
	bool AddBlockID(const FName& InBlockIDName, const int32& InVariant = 0, const bool& bIsBlocking = true)
	{
		for(const auto& Item : BlockIDs)
		{
			if(Item.BlockID == InBlockIDName)
			{
				return false;
			}
		}
		FDigumWorldBlockID ID(InBlockIDName, InVariant);
		ID.bIsBlocking = bIsBlocking;
		BlockIDs.Add(ID);
		return true;
	}

	bool AddBlockID(const FDigumWorldBlockID& InBlockID)
	{
		for(const auto& Item : BlockIDs)
		{
			if(Item.BlockID == InBlockID.BlockID)
			{
				return false;
			}
		}
		BlockIDs.Add(InBlockID);
		return true;
	}

	bool IsOccupied() const
	{
		for(auto& ID : BlockIDs)
		{
			if(ID.bIsBlocking == true) return true;
		}

		return false;
	}
	bool IsDirectSurfaceBlock() const { return bIsDirectSurfaceBlock; }
	bool IsInitialized() const { return X != -1 && Y != -1;};
};

USTRUCT()
struct FDigumWorldProceduralCoordinateArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FDigumWorldProceduralCoordinate> Coordinates;

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

	FDigumWorldProceduralCoordinate* GetGlobalCoordinate(const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchy)
	{
		for(FDigumWorldProceduralCoordinate& Coordinate : Coordinates)
		{
			if(Coordinate.GlobalX == InGlobalX && Coordinate.GlobalY == InGlobalY && Coordinate.Hierarchy == InHierarchy)
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
			for(auto& BlockID : Coordinate.BlockIDs)
			{
				const FName IDName = BlockID.BlockID;
				OutMappedCoordinates.FindOrAdd(IDName).AddCoordinate(Coordinate);
			}
			// 
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

	UPROPERTY()
	int32 SectionWidth = 0;

	
	UPROPERTY()
	int32 SectionHeight = 0;

	UPROPERTY()
	int32 HierarchyCount = 0;

	
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
	int32 GetSectionWidth() const { return SectionWidth; };

	
	bool operator==(const FDigumWorldProceduralSection& InSection) const
	{
		return GetX() == InSection.GetX() && GetY() == InSection.GetY();
	}
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
	FName Seed = TEXT("Hello World");
	
	UPROPERTY(EditAnywhere)
	int32 SectionWidth = 8;

	UPROPERTY(EditAnywhere)
	int32 SectionHeight = 8;

	// Spans both negative and positive values
	UPROPERTY(EditAnywhere)
	int32 SectionCount_HorizontalAxis = 64;

	// Spans both negative and positive values
	UPROPERTY(EditAnywhere)
	int32 SectionCount_VerticalAxis = 64;
	
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

	UPROPERTY()
	int32 HierarchyIndex;
};

/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldGenerator : public UObject
{
	GENERATED_BODY()
private:
	static bool GetBooleanViaPerlinNoise(const float& InX, const FRandomStream& InRandomStream, const float& NormalizedThreshold = 0.5f);
	static float GetPerlinNoiseValue1D(const float InX, const FRandomStream& InRandomStream);
	static float GetPerlinNoiseValue2D(const float InX, const int32 InY, const FRandomStream& InRandomStream);
	static float GetPerlinNoiseValue3D(const float InX, const int32 InY, const int32 InZ, const FRandomStream& InRandomStream);
	
	static void GenerateUndergroundVeins(FDigumWorldProceduralMap& OutMap);
	// static void GenerateTrees(FDigumWorldProceduralMap& OutMap);
	// static void GenerateFoliage(FDigumWorldProceduralMap& OutMap);
	static float GetGroundNoise(const float& X, const FRandomStream& InRandomStream);
	static float QuadraticBezier(float t, float P0, float P1, float P2);
	static float CubicBezier(float t, float P0, float P1, float P2, float P3);
	static TArray<float> SmoothTerrain(const TArray<float>& GroundCurve, int32 SmoothingPasses);
	
	static float NormalizeNoiseValue(const float InNoiseValue);
	static FName GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>>& OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks);
	static bool GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights, const FVector2D& Seed);
	static bool GetWeightedBlockID(const float InNoiseValue,const TArray<FDigumWorldProceduralBlock>& Blocks, FName& OutBlockIDName, int32& OutVariant);
	
	static TArray<float> GenerateGroundCurve(const int32& InWidth, const int32& InHeight, const int32& SectionX, const FRandomStream& InRandomStream);
	static bool IsSurfacePoint(const int32& PositionX, const int32& PositionY, const TArray<float>& GroundCurve, const int32& InWidth, const int32& InSectionX);
	static bool IsAreaAvailable(TArray<FDigumWorldProceduralBlock> InPlacedBlocks, const int32& InStartX, const int32& InStartY, const int32& InOriginX, const int32& InOriginY, const int32& InWidth, const int32& InHeight);
	static FName GetWeightedBlockID(const float InNoiseValue, const TArray<TPair<float, float>>& CumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks)
	{
		for (int32 i = 0; i < CumulativeWeights.Num(); ++i)
		{
			const TPair<float, float>& Range = CumulativeWeights[i];
			if (InNoiseValue >= Range.Key && InNoiseValue < Range.Value)
			{
				return Blocks[i].BlockID;
			}
		}

		return NAME_None; // Default case if no match found
	}

	static void GetCumulativeWeights(const TArray<float>& NormalizedWeights, TArray<TPair<float, float>>& OutCumulativeWeights)
	{
		float CumulativeSum = 0.0f;

		for (const auto& Weight : NormalizedWeights)
		{
			float StartRange = CumulativeSum;
			CumulativeSum += Weight;
			float EndRange = CumulativeSum;

			OutCumulativeWeights.Add(TPair<float, float>(StartRange, EndRange));
		}
	}
	static void NormalizeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<float>& NormalizedWeights)
	{
		float TotalWeight = 0.0f;
		for (const auto& Block : Blocks)
		{
			TotalWeight += Block.Weight;
		}

		for (const auto& Block : Blocks)
		{
			NormalizedWeights.Add(Block.Weight / TotalWeight);
		}
	}
	static bool IsInPlacedBlock(const int32& InGlobalX, const int32& InGlobalY, const int32& InWidth, const int32& InHeight, const int32& InHierarchyIndex, const TArray<FDigumWorldProceduralPlacedBlocks>& InPlacedBlocks);
	static FDigumWorldProceduralCoordinate* GetCoordinateInSection(const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchyIndex, FDigumWorldProceduralSection* InSection);
	static bool CanPlaceSizedBlock(const EDigumWorldSurfaceAttachType& AttachType, FDigumWorldProceduralCoordinate* InCoordinate, const int32& InPlaceableWidth, const int32& InPlaceableHeight, FDigumWorldProceduralSection* InMainSection, FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection);
	static bool IsBlockOccupied(const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchyIndex, FDigumWorldProceduralSection* InSection);
	static bool IsSurfaceBlock(const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchyIndex, FDigumWorldProceduralSection* InSection);
	// static void GenerateSection(const )
public:
	static bool GenerateTerrainSection(const FDigumWorldMap &InMap, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection);
	static bool GenerateTerrainSection(const FName& InSeed, const int32& InSectionX, const int32& InSectionY,
                        const int32& InSectionWidth, const int32& InSectionHeight,
                        const int32& InSectionCount_HorizontalAxis, const int32& InSectionCount_VerticalAxis,
                        const int32& InNumberOfHierarchies,
                        const FDigumWorldProceduralDefinition& InProceduralDefinition,
                        FDigumWorldProceduralSection& OutSection);
	static bool GenerateTerrainSection(const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream, 
							   const int32& NumOfHierarchies,  const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection);
	static bool GenerateTerrainSection(const FName& InSeed, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralSection& OutSection);

	static bool CreateSection(const FDigumWorldMap &InMap, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection);
	static bool GenerateTrees(const FName& InSeedName, TArray<FDigumWorldProceduralSection>& InSectionArray, const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks);
	static bool GenerateFoliage(const FName& InSeedName, TArray<FDigumWorldProceduralSection>& InSectionArray, const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks);
	static bool GenerateFoliage(const FName& InSeedName, FDigumWorldProceduralSection* InSection, const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks);
	static bool GenerateTrees(const FName& InSeedName, FDigumWorldProceduralSection* InSection, const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralPlacedBlocks>& InPlacedBlocks,  FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection);
	static void GenerateWorldMap(const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralMap& OutMap);
	static void MarkForFoliage(FDigumWorldProceduralCoordinate* InCoordinate);
	static void CheckAndSetNeighbors(FDigumWorldProceduralSection* InSection, const int32& NumOfHierarchies, FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection, int32 InLocalSectionWidth, int32 InLocalSectionHeight);

	/*static bool GetCumulativeWeights(TArray<TPair<float, float>>& OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks, const FVector2D& Seed)
	{
		if (Blocks.IsEmpty()) return false;

		float TotalWeight = 0.0f;
		float CumulativeSum = 0.0f;

		for (const auto& Block : Blocks)
		{
			TotalWeight += Block.Weight;
		}

		for (const auto& Block : Blocks)
		{
			float StartRange = CumulativeSum;
			CumulativeSum += Block.Weight;
			float EndRange = CumulativeSum;

			// Normalize the ranges
			StartRange /= TotalWeight;
			EndRange /= TotalWeight;

			// Apply a small Perlin noise-based perturbation if weights are equal to avoid always defaulting to the first item
			float Perturbation = FMath::PerlinNoise2D(FVector2D(StartRange, Seed.Y)) * 0.001f;
			StartRange += Perturbation;
			EndRange += Perturbation;

			OutCumulativeWeights.Add(TPair<float, float>(StartRange, EndRange));
		}

		return true;
	}*/
};
	