// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Procedural/DigumWorldGenerator.h"

#include "Asset/DigumWorldProceduralAsset.h"
#include "Algo/MinElement.h"
#include "Procedural/DigumWorldMap.h"


float UDigumWorldGenerator::NormalizeNoiseValue(const float InNoiseValue)
{
	return (InNoiseValue + 1.0f) * 0.5f;
}

FName UDigumWorldGenerator::GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>>& CumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks)
{
	for(int32 i = 0; i < Blocks.Num(); i++)
	{
		const TPair<float, float>& Range = CumulativeWeights[i];
		if (InNoiseValue >= Range.Key && InNoiseValue < Range.Value)
		{
			return Blocks[i].BlockID;
		}
	}
	
	return NAME_None;
}

bool UDigumWorldGenerator::GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights, const FVector2D& Seed)
{
	if (Blocks.IsEmpty()) return false;

	float TotalWeight = 0.0f;
	float CumulativeSum = 0.0f;

	for (const auto& Block : Blocks)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Cumulative Weight %s"), *Block.BlockID.ToString())
		TotalWeight += Block.Weight;
	}

	for (int32 Index = 0; Index < Blocks.Num(); ++Index)
	{
		const auto& Block = Blocks[Index];
		float StartRange = CumulativeSum;
		CumulativeSum += Block.Weight;
		float EndRange = CumulativeSum;

		// Normalize the ranges
		StartRange /= TotalWeight;
		EndRange /= TotalWeight;

		// Apply a small Perlin noise-based perturbation to the EndRange
		float Perturbation = FMath::PerlinNoise2D(FVector2D(Seed.X + Index, Seed.Y)) * 0.001f;
		EndRange += Perturbation;

		OutCumulativeWeights.Add(TPair<float, float>(StartRange, EndRange));
	}

	return true;
}


bool UDigumWorldGenerator::GetWeightedBlockID(const float InNoiseValue,
	const TArray<FDigumWorldProceduralBlock>& Blocks, FName& OutBlockIDName, int32& OutVariant)
{
	// UE_LOG(LogTemp, Warning, TEXT("Noise Value: %f"), InNoiseValue);
	float BlockWeight = 0;
	for(auto& Block : Blocks)
	{
		BlockWeight += Block.Weight;
	}

	float WeightedBlockNoise = InNoiseValue * BlockWeight;
	
	for(const auto& Block : Blocks)
	{
		WeightedBlockNoise -= Block.Weight;
		
		if(WeightedBlockNoise <= 0)
		{
			float VariantWeight = 0;

			for(const auto& Variant : Block.Variants)
			{
				VariantWeight += Variant.Weight;
			}

			float WeightedVariantNoise = InNoiseValue * VariantWeight;
			int32 VariantIndex = 0;
			for(int32 i = 0; i < Block.Variants.Num(); i++)
			{
				const auto& Variant = Block.Variants[i];
				WeightedVariantNoise -= Variant.Weight;
				if(WeightedVariantNoise <= 0)
				{
					VariantIndex = i;
					break;
				}
			}
			OutVariant = VariantIndex;
			OutBlockIDName = Block.BlockID;
			return true;
		}
	}

	return false;
}


TArray<float> UDigumWorldGenerator::GenerateGroundCurve(const int32& InWidth, const int32& InHeight,
                                                        const int32& SectionX, const FRandomStream& InRandomStream)
{
	TArray<float> GroundCurve;
	GroundCurve.SetNum(InWidth);

	const int32 VerticalCenter = InHeight / 2;
	const float AmplitudeScale = 0.1f; // Adjustable amplitude scale
	const float FrequencyScale = 0.007f; // Adjustable frequency
    const float Amplitude = InHeight / 2 * AmplitudeScale; // Adjustable amplitude
    const float NoiseScale = FrequencyScale; // Adjustable frequency

    // Generate Perlin noise values for the base heights
    for (int32 x = 0; x < InWidth; x++)
    {
        float NoiseValue = GetPerlinNoiseValue1D(static_cast<float>(SectionX * InWidth + x) * NoiseScale, InRandomStream);
        GroundCurve[x] = VerticalCenter + NoiseValue;
    }

    // Determine terrain types
    for (int32 x = 0; x < InWidth; x += 10) // Divide terrain into segments of 10 units
    {
    	float TerrainTypeNoiseValue = GetPerlinNoiseValue1D(static_cast<float>(SectionX * InWidth + x), InRandomStream);

    	if (TerrainTypeNoiseValue < -0.3f)
    	{
    		// Flat terrain
    		for (int32 i = 0; i < 10 && x + i < InWidth; i++)
    		{
    			GroundCurve[x + i] = VerticalCenter;
    		}
    	}
    	else if (TerrainTypeNoiseValue < 0.3f)
    	{
    		// Smaller hill
    		for (int32 i = 0; i < 10 && x + i < InWidth; i++)
    		{
    			float NoiseValue = GetPerlinNoiseValue1D(static_cast<float>(SectionX * InWidth + x + i) * NoiseScale, InRandomStream);
    			GroundCurve[x + i] = VerticalCenter + NoiseValue * Amplitude / 10;// Smaller amplitude
    		}
    	}
        // Otherwise, keep the original hill shape
    }

    // Smooth the curve using cubic Bézier curves
    for (int32 x = 0; x < InWidth; x += 3)
    {
        int32 P0 = x;
        int32 P1 = FMath::Clamp(x + 1, 0, InWidth - 1);
        int32 P2 = FMath::Clamp(x + 2, 0, InWidth - 1);
        int32 P3 = FMath::Clamp(x + 3, 0, InWidth - 1);

        for (int32 i = 0; i <= 3 && x + i < InWidth; i++)
        {
            float t = static_cast<float>(i) / 3.0f; // Normalize t to [0, 1] within the segment
            GroundCurve[x + i] = CubicBezier(t, GroundCurve[P0], GroundCurve[P1], GroundCurve[P2], GroundCurve[P3]);
        }
    }

    // Apply smoothing to eliminate floating artifacts
    GroundCurve = SmoothTerrain(GroundCurve, 4);
	return GroundCurve;
}

bool UDigumWorldGenerator::IsSurfacePoint(const int32& PositionX, const int32& PositionY,
	const TArray<float>& GroundCurve, const int32& InWidth, const int32& InSectionX)
{
	const int32 LocalX = PositionX - (InSectionX * InWidth);
	if (LocalX >= 0 && LocalX < InWidth)
	{
		return PositionY == static_cast<int32>(GroundCurve[LocalX]);
	}
	
	return false;
}


bool UDigumWorldGenerator::IsAreaAvailable(TArray<FDigumWorldProceduralBlock> InPlacedBlocks, const int32& InStartX, const int32& InStartY,
	const int32& InOriginX, const int32& InOriginY, const int32& InWidth, const int32& InHeight)
{

	for (const auto& Block : InPlacedBlocks)
	{
		if (InStartX < InOriginX + Block.Width && InStartX + InWidth > InOriginX &&
			InStartY < InOriginY + Block.Height && InStartY + InHeight > InOriginY)
		{
			return false;
		}
	}
	return true;
}


bool UDigumWorldGenerator::IsInPlacedBlock(const int32& InGlobalX, const int32& InGlobalY, const int32& InWidth,
	const int32& InHeight,  const int32& InHierarchyIndex, const TArray<FDigumWorldProceduralPlacedBlocks>& InPlacedBlocks)
{
	const int32 OriginX = InGlobalX;
	const int32 OriginY = InGlobalY;
	const int32 HalfWidth = InWidth / 2;
	const int32 StartX = OriginX - HalfWidth;
	const int32 EndX = OriginX + HalfWidth;
	const int32 StartY = OriginY;
	const int32 EndY = OriginY + InHeight - 1;
	const int32 HierarchyIndex = InHierarchyIndex;


	for(auto& Block : InPlacedBlocks)
	{
		for(int32 x = StartX; x <= EndX; x++)
		{
			for(int32 y = StartY; y <= EndY; y++)
			{
				if(Block.OriginX == x && Block.OriginY == y && Block.HierarchyIndex == InHierarchyIndex)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool UDigumWorldGenerator::CanPlaceSizedBlock(FDigumWorldProceduralCoordinate* InCoordinate,
                                              const int32& InPlaceableWidth, const int32& InPlaceableHeight, FDigumWorldProceduralSection* InLeftSection,
                                              FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection,  FDigumWorldProceduralSection* InBottomSection)
{
	if(InCoordinate == nullptr) return false;
	
	const int32 OriginX = InCoordinate->GlobalX;
	const int32 OriginY = InCoordinate->GlobalY;
	const int32 HalfWidth = InPlaceableWidth / 2;
	const int32 StartX = OriginX - HalfWidth;
	const int32 EndX = OriginX + HalfWidth;
	const int32 StartY = OriginY;
	const int32 EndY = OriginY + InPlaceableHeight - 1;
	const int32 HierarchyIndex = InCoordinate->Hierarchy;

	// bool bCanPlaceSizedBlock = true;
	bool bOccupied = false;
	for(int32 x = StartX; x <= EndX; x++)
	{
		for(int32 y = StartY; y <= EndY; y++)
		{
			
			const bool bTop = IsBlockOccupied(x, y, HierarchyIndex, InTopSection);
			const bool bRight = IsBlockOccupied(x, y, HierarchyIndex, InRightSection);
			const bool bLeft = IsBlockOccupied(x, y, HierarchyIndex, InLeftSection);
			// const bool bBottom = IsBlockOccupied(x, y, HierarchyIndex, InBottomSection);

			if(bTop || bRight || bLeft) return false;
		}
	}

	return !bOccupied;
}

bool UDigumWorldGenerator::IsBlockOccupied(const int32& InGlobalX, const int32& InGlobalY, const int32& InHierarchyIndex,
	FDigumWorldProceduralSection* InSection)
{
	if(InSection)
	{
		FDigumWorldProceduralCoordinateArray* Array = InSection->GetCoordinateArray();
		if(Array)
		{
			FDigumWorldProceduralCoordinate* Coordinate = Array->GetGlobalCoordinate(InGlobalX, InGlobalY, InHierarchyIndex);
			if(Coordinate && Coordinate->IsInitialized())
			{
				if(Coordinate->IsOccupied()) return true;
			}
		}
	}

	return false;
}

bool UDigumWorldGenerator::GenerateTerrainSection(const FDigumWorldMap& InMap, const int32& InSectionX,
                                                  const int32& InSectionY, const FDigumWorldProceduralDefinition& ProceduralDefinition,
                                                  FDigumWorldProceduralSection& OutSection)
{
	// UE_LOG(LogTemp, Warning, TEXT("Generating Section %i, %i"), InSectionX, InSectionY);
	const FName SeedName = InMap.Seed;
	const int32 SectionWidth = InMap.SectionWidth;
	const int32 SectionHeight = InMap.SectionHeight;
	const int32 NumberOfHierarchies = InMap.NumberOfHierarchies;
	
	/*const TArray<FDigumWorldProceduralBlock> TerrainBlocks = ProceduralAsset->GetTerrainBlocks();
	TArray<TPair<float, float>> TerrainCumulativeWeights;
	TArray<TPair<float, float>> GrassCumulativeWeights;
	const bool bHasTerrainCumulativeWeights = GetCumulativeWeights(TerrainBlocks, TerrainCumulativeWeights);*/

	/*if(!bHasTerrainCumulativeWeights)
	{
		UE_LOG(LogTemp, Error, TEXT("No Cumulative Weights"));
		return false;
	}*/
	/*if(ProceduralAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Procedural Asset is null"));
		return false;
	}*/
	const FRandomStream RandomStream(SeedName);
	const int32 MapWidth = InMap.GetTotalSectionWidth();
	const int32 MapHeight = InMap.GetTotalSectionHeight();

	// UE_LOG(LogTemp, Warning, TEXT("Map Width: %d, Map Height: %d"), MapWidth, MapHeight);
	FDigumWorldProceduralSection TempSection;

	const bool bResult = GenerateTerrainSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream,  NumberOfHierarchies, ProceduralDefinition, TempSection);
	OutSection =  TempSection;
	return bResult;
}


bool UDigumWorldGenerator::GenerateTerrainSection(const FName& InSeed, const int32& InSectionX, const int32& InSectionY,
	const int32& InSectionWidth, const int32& InSectionHeight, const int32& InSectionCount_HorizontalAxis, const int32& InSectionCount_VerticalAxis,  
	const int32& InNumberOfHierarchies, const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection)
{
	const FVector2D CumulativeSeed = FVector2D(InSectionX, InSectionY);
	const FName Seed = InSeed;
	const int32 SectionWidth = InSectionWidth;
	const int32 SectionHeight = InSectionHeight;
	const int32 NumberOfHierarchies = InNumberOfHierarchies;
	const TArray<FDigumWorldProceduralBlock> Blocks = InProceduralDefinition.GetTerrainBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights, CumulativeSeed);
	if(!bHasCumulativeWeights)
	{
		UE_LOG(LogTemp, Error, TEXT("No Cumulative Weights"));
		return false;
	}
	const FRandomStream RandomStream(Seed);
	
	const int32 MapWidth = InSectionCount_HorizontalAxis * InSectionWidth;
	const int32 MapHeight = InSectionCount_VerticalAxis * InSectionHeight;

	FDigumWorldProceduralSection TempSection;

	const bool bResult = GenerateTerrainSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream, NumberOfHierarchies, InProceduralDefinition, TempSection);
	OutSection =  TempSection;
	return bResult;
}

bool UDigumWorldGenerator::GenerateTerrainSection(const FName& InSeed, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralSection& OutSection)
{
	const UDigumWorldProceduralAsset* ProceduralAsset = InRules.GetProceduralAsset();
	const FName Seed = InSeed;
	const int32 SectionWidth = InRules.SectionWidth;
	const int32 SectionHeight = InRules.SectionHeight;
	const int32 NumberOfHierarchies = InRules.NumberOfHierarchies;
	const int32 SectionHorizontalAxis = InRules.SectionCount_HorizontalAxis;
	const int32 SectionVerticalAxis = InRules.SectionCount_VerticalAxis;
	const FDigumWorldProceduralDefinition Definition = ProceduralAsset->ProceduralDefinition;

	return GenerateTerrainSection(Seed, InSectionX, InSectionY, SectionWidth, SectionHeight, SectionHorizontalAxis, SectionVerticalAxis, NumberOfHierarchies, Definition, OutSection);
	
	/*
	const FVector2D CumulativeSeed = FVector2D(InSectionX, InSectionY);	
	const int32 Seed = InSeed;
	const int32 SectionWidth = InRules.SectionWidth;
	const int32 SectionHeight = InRules.SectionHeight;
	const int32 NumberOfHierarchies = InRules.NumberOfHierarchies;
	const UDigumWorldProceduralAsset* ProceduralAsset = InRules.GetProceduralAsset();
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->ProceduralDefinition.GetTerrainBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights, CumulativeSeed);
	if(!bHasCumulativeWeights)
	{
		UE_LOG(LogTemp, Error, TEXT("No Cumulative Weights"));
		return false;
	}
	if(ProceduralAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Procedural Asset is null"));
		return false;
	}
	const FRandomStream RandomStream(Seed);
	
	const int32 MapWidth = InRules.SectionCount_HorizontalAxis * SectionWidth;
	const int32 MapHeight = InRules.SectionCount_VerticalAxis * SectionHeight;

	FDigumWorldProceduralSection TempSection;

	const bool bResult = GenerateSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream, NumberOfHierarchies, ProceduralAsset->ProceduralDefinition, TempSection);
	OutSection =  TempSection;
	return bResult;*/
}

bool UDigumWorldGenerator::CreateSection(const FDigumWorldMap& InMap, const int32& InSectionX, const int32& InSectionY,
	const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection)
{
	FDigumWorldProceduralSection
		MainSection,
		TopSection,
		BottomSection,
		RightSection,
		LeftSection;
	
	// Generate sections
	const bool bHasMainSection = GenerateTerrainSection(InMap, InSectionX, InSectionY, InProceduralDefinition, MainSection);
	const bool bHasTopSection = GenerateTerrainSection(InMap, InSectionX, InSectionY - 1, InProceduralDefinition, TopSection);
	const bool bHasBottomSection = GenerateTerrainSection(InMap, InSectionX, InSectionY + 1, InProceduralDefinition, BottomSection);
	const bool bHasLeftSection = GenerateTerrainSection(InMap, InSectionX - 1, InSectionY, InProceduralDefinition, LeftSection);
	const bool bHasRightSection = GenerateTerrainSection(InMap, InSectionX + 1, InSectionY, InProceduralDefinition, RightSection);

	// Find Surfaces and Edges
	CheckAndSetNeighbors(&MainSection, InMap.NumberOfHierarchies, &LeftSection, &RightSection, &TopSection, &BottomSection, InMap.GetLocalSectionWidth(), InMap.GetLocalSectionHeight());

	// Foliage
	TArray<FDigumWorldProceduralBlock> FoliagePlacedBlocks;
	GenerateFoliage(InMap.Seed, &MainSection, InProceduralDefinition, FoliagePlacedBlocks);

	// Trees
	TArray<FDigumWorldProceduralPlacedBlocks> TreePlacedBlocks;
	GenerateTrees(InMap.Seed, &MainSection, InProceduralDefinition, TreePlacedBlocks, &LeftSection, &RightSection, &TopSection, &BottomSection);
	
	
	OutSection = MainSection;

	return bHasMainSection;
}

bool UDigumWorldGenerator::GenerateTerrainSection(const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream,
                                                  const int32& NumOfHierarchies,  const FDigumWorldProceduralDefinition& InProceduralDefinition, FDigumWorldProceduralSection& OutSection)
{
	// if(ProceduralAsset == nullptr) return false;
	
	const TArray<FDigumWorldProceduralBlock> TerrainBlocks = InProceduralDefinition.GetTerrainBlocks();
	// FVector2D CumulativeSeed(InSectionX, InSectionY);

	TArray<float> NormalizedWeights;
	NormalizeWeights(TerrainBlocks, NormalizedWeights);

	TArray<TPair<float, float>> TerrainCumulativeWeights;
	GetCumulativeWeights(NormalizedWeights, TerrainCumulativeWeights);

	bool bResult = false;
	OutSection = FDigumWorldProceduralSection(InSectionX, InSectionY);
	TArray<float> GroundCurve = GenerateGroundCurve(InWidth, InMapHeight, InSectionX, InRandomStream);
	for(int32 i = 0; i < NumOfHierarchies; i++)
	{
		const int32 HierarchyIndex = i - (NumOfHierarchies -1);
		
		for(int32 x = 0; x < InWidth; x++)
		{
			for(int32 y = 0; y < InHeight; y++)
			{
				const int32 PositionX = InSectionX * InWidth + x;
				const int32 PositionY = InSectionY * InHeight + y;
				const float NoiseValue= GetPerlinNoiseValue3D(PositionX, PositionY, HierarchyIndex, InRandomStream);
				const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);
				FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate(x, y, PositionX, PositionY);
				Coordinate.Hierarchy = HierarchyIndex;
				Coordinate.NoiseValue = NoiseValue;
				
				if(PositionY < GroundCurve[x])
				{
					// Above ground, surfaces
					
				}
				else // Below ground
				{
					const FName TerrainBlockID = GetWeightedBlockID(NormalizedNoise, TerrainCumulativeWeights, TerrainBlocks);
					const FDigumWorldProceduralBlock* Block = InProceduralDefinition.GetBlockInfo(TerrainBlockID, TerrainBlocks);
					bool bBlocking = true;
					if(Block)
					{
						bBlocking = Block->bIsBlocking;
					}
					// TODO Set Terrain Variant
					Coordinate.AddBlockID(TerrainBlockID, 0, bBlocking);
				}

				OutSection.AddCoordinate(Coordinate);
				bResult = true; // Set to true if we have at least one coordinate
			}
		}
	}
	
	return bResult;
}



bool UDigumWorldGenerator::GenerateTrees(const FName& InSeedName, TArray<FDigumWorldProceduralSection>& InSectionArray,
                                         const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks)
{
	const TArray<FDigumWorldProceduralBlock> TreesBlocks = InProceduralDefinition.GetTreesBlock();
	
	TArray<float> NormalizedWeights;
	NormalizeWeights(TreesBlocks, NormalizedWeights);

	TArray<TPair<float, float>> CumulativeWeights;
	GetCumulativeWeights(NormalizedWeights, CumulativeWeights);
	FRandomStream RandomStream(InSeedName);
	for(int32 i = 0; i < InSectionArray.Num(); i++)
	{
		FDigumWorldProceduralSection& Section = InSectionArray[i];
	
		FDigumWorldProceduralCoordinateArray* Coordinates = InSectionArray[i].GetCoordinateArray();
		for(int32 j = 0; j < Coordinates->CoordinateCount(); j++)
		{
			FDigumWorldProceduralCoordinate* Coordinate = Coordinates->GetCoordinate(j);
			if(Coordinate != nullptr && Coordinate->IsDirectSurfaceBlock())
			{
				const int32 PositionX = Coordinate->GlobalX;
				const int32 PositionY = Coordinate->GlobalY;
				const int32 HierarchyIndex = Coordinate->Hierarchy;
				const float NoiseValue= GetPerlinNoiseValue3D(PositionX, PositionY, HierarchyIndex, RandomStream);
				const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);
				FName TreeBlockID = NAME_None;
				int32 TreeVariant = 0;
				GetWeightedBlockID(NormalizedNoise, TreesBlocks, TreeBlockID, TreeVariant);
				
				Coordinate->AddBlockID(TreeBlockID, TreeVariant);
			}
		}
	}
	return true;
}


bool UDigumWorldGenerator::GenerateFoliage(const FName& InSeedName,
	TArray<FDigumWorldProceduralSection>& InSectionArray,  const FDigumWorldProceduralDefinition& InProceduralDefinition,
	TArray<FDigumWorldProceduralBlock>& PlaceBlocks)
{
	const TArray<FDigumWorldProceduralBlock> GrassBlocks = InProceduralDefinition.GetGrassBlocks();
	/*TArray<TPair<float, float>> GrassCumulativeWeights;
	const bool bHasGrassCumulativeWeights = GetCumulativeWeights(GrassBlocks, GrassCumulativeWeights);
	if(!bHasGrassCumulativeWeights)
	{
		UE_LOG(LogTemp, Error, TEXT("No Cumulative Weights"));
		return false;
	}*/
	
	FRandomStream RandomStream(InSeedName);
	for(int32 i = 0; i < InSectionArray.Num(); i++)
	{
		FDigumWorldProceduralSection& Section = InSectionArray[i];
		FVector2D CumulativeSeed = FVector2D(Section.GetX(), Section.GetY());
		TArray<TPair<float, float>> CumulativeWeights;
		const bool bHasCumulativeWeights = GetCumulativeWeights(GrassBlocks, CumulativeWeights, CumulativeSeed);
		FDigumWorldProceduralCoordinateArray* Coordinates = InSectionArray[i].GetCoordinateArray();
		for(int32 j = 0; j < Coordinates->CoordinateCount(); j++)
		{
			FDigumWorldProceduralCoordinate* Coordinate = Coordinates->GetCoordinate(j);
			if(Coordinate != nullptr && Coordinate->IsDirectSurfaceBlock())
			{
				const int32 PositionX = Coordinate->GlobalX;
				const int32 PositionY = Coordinate->GlobalY;
				const int32 HierarchyIndex = Coordinate->Hierarchy;
				const float NoiseValue= GetPerlinNoiseValue3D(PositionX, PositionY, HierarchyIndex, RandomStream);
				const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);
				
				FName OutBlockID = NAME_None;
				int32 OutVariant = 0;
				
				GetWeightedBlockID(NormalizedNoise, GrassBlocks, OutBlockID, OutVariant);
				UE_LOG(LogTemp, Warning, TEXT("ID: %s Variant Value: %i"),*OutBlockID.ToString(), OutVariant);
				Coordinate->AddBlockID(OutBlockID, OutVariant, false);
			}
		}
	}
	return true;
}

bool UDigumWorldGenerator::GenerateFoliage(const FName& InSeedName, FDigumWorldProceduralSection* InSection,
	const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks)
{
	if(InSection == nullptr) return false;
	
	const TArray<FDigumWorldProceduralBlock> GrassBlocks = InProceduralDefinition.GetGrassBlocks();
	const FVector2D CumulativeSeed = FVector2D(InSection->GetX(), InSection->GetY());
	const FRandomStream RandomStream(InSeedName);
	
	TArray<TPair<float, float>> CumulativeWeights;
	const bool bHasCumulativeWeights = GetCumulativeWeights(GrassBlocks, CumulativeWeights, CumulativeSeed);
	FDigumWorldProceduralCoordinateArray* Coordinates = InSection->GetCoordinateArray();
	for(int32 j = 0; j < Coordinates->CoordinateCount(); j++)
	{
		FDigumWorldProceduralCoordinate* Coordinate = Coordinates->GetCoordinate(j);
		if(Coordinate != nullptr && Coordinate->IsDirectSurfaceBlock())
		{
			const int32 PositionX = Coordinate->GlobalX;
			const int32 PositionY = Coordinate->GlobalY;
			const int32 HierarchyIndex = Coordinate->Hierarchy;
			const float NoiseValue= GetPerlinNoiseValue3D(PositionX, PositionY, HierarchyIndex, RandomStream);
			const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);
				
			FName OutBlockID = NAME_None;
			int32 OutVariant = 0;
				
			GetWeightedBlockID(NormalizedNoise, GrassBlocks, OutBlockID, OutVariant);
			UE_LOG(LogTemp, Warning, TEXT("ID: %s Variant Value: %i"),*OutBlockID.ToString(), OutVariant);
			Coordinate->AddBlockID(OutBlockID, OutVariant);
		}
	}

	return true;
}

bool UDigumWorldGenerator::GenerateTrees(const FName& InSeedName, FDigumWorldProceduralSection* InSection,
	const FDigumWorldProceduralDefinition& InProceduralDefinition, TArray<FDigumWorldProceduralPlacedBlocks>& InPlacedBlocks,
	FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection,
	FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection)
{
	if(InSection == nullptr) return false;
	const TArray<FDigumWorldProceduralBlock> TreesBlock = InProceduralDefinition.GetTreesBlock();
	TArray<float> NormalizedWeights;
	NormalizeWeights(TreesBlock, NormalizedWeights);

	TArray<TPair<float, float>> CumulativeWeights;
	GetCumulativeWeights(NormalizedWeights, CumulativeWeights);
	FRandomStream RandomStream(InSeedName);
	
	FDigumWorldProceduralCoordinateArray* Coordinates = InSection->GetCoordinateArray();
	for(int32 j = 0; j < Coordinates->CoordinateCount(); j++)
	{
		FDigumWorldProceduralCoordinate* Coordinate = Coordinates->GetCoordinate(j);
		if(Coordinate != nullptr && Coordinate->IsDirectSurfaceBlock())
		{
			const int32 PositionX = Coordinate->GlobalX;
			const int32 PositionY = Coordinate->GlobalY;
			const int32 HierarchyIndex = Coordinate->Hierarchy;
			const float NoiseValue= GetPerlinNoiseValue3D(PositionX, PositionY, HierarchyIndex, RandomStream);
			const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);

			
			FName TreeBlockID = NAME_None;
			int32 TreeVariant = 0;
			GetWeightedBlockID(NormalizedNoise, TreesBlock, TreeBlockID, TreeVariant);
			
			const FDigumWorldProceduralBlock* Block = InProceduralDefinition.GetBlockInfo(TreeBlockID, TreesBlock);
			if(Block)
			{
				const int32 Width = Block->Width;
				const int32 Height = Block->Height;
				// if(Width > 0 && Height)

				if(!IsInPlacedBlock(PositionX, PositionY, Width, Height, HierarchyIndex, InPlacedBlocks))
				{
					if(CanPlaceSizedBlock(Coordinate, Width, Height, InLeftSection, InRightSection, InTopSection, InBottomSection))
					{
						FDigumWorldProceduralPlacedBlocks PlacedBlocks = FDigumWorldProceduralPlacedBlocks();
						PlacedBlocks.OriginX = PositionX;
						PlacedBlocks.OriginY = PositionY;
						PlacedBlocks.HierarchyIndex = HierarchyIndex;
					
						Coordinate->AddBlockID(TreeBlockID, TreeVariant);
						InPlacedBlocks.Add(PlacedBlocks);		
					}
				}

				/*else
				{
					return false;
				}*/
			}
				
			
		}
	}
	
	return true;
}

void UDigumWorldGenerator::GenerateWorldMap(const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralMap& OutMap)
{
	const int32 Seed = InRules.Seed;
	const int32 SectionWidth = InRules.SectionWidth;
	const int32 SectionHeight = InRules.SectionHeight;
	const int32 NumberOfHierarchies = InRules.NumberOfHierarchies;
	const UDigumWorldProceduralAsset* ProceduralAsset = InRules.GetProceduralAsset();
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->ProceduralDefinition.GetTerrainBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	// TODO change 0 cumulative seed
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights, FVector2D(0,0));
	if(!bHasCumulativeWeights)
	{
		UE_LOG(LogTemp, Error, TEXT("No Cumulative Weights"));
		return;
	}
	if(ProceduralAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Procedural Asset is null"));
		return;
	}
	const FRandomStream RandomStream(Seed);
	FDigumWorldProceduralMap TempMap = FDigumWorldProceduralMap();
	
	const int32 MapWidth = InRules.SectionCount_HorizontalAxis * SectionWidth;
	const int32 MapHeight = InRules.SectionCount_VerticalAxis * SectionHeight;
	
	for(int32 h = 0; h < InRules.SectionCount_HorizontalAxis; h++)
	{
		for(int32 v = 0; v < InRules.SectionCount_VerticalAxis; v++)
		{
			FDigumWorldProceduralSection OutSection = FDigumWorldProceduralSection(h, v);
			if(GenerateTerrainSection(MapWidth,MapHeight, h, v, SectionWidth, SectionHeight, RandomStream, NumberOfHierarchies, ProceduralAsset->ProceduralDefinition, OutSection))
			{
				TempMap.AddSection(OutSection);
			}
		}
	}

	OutMap = TempMap;
}

void UDigumWorldGenerator::MarkForFoliage(FDigumWorldProceduralCoordinate* InCoordinate)
{
	if(InCoordinate)
	{
		UE_LOG(LogTemp, Log, TEXT("Coordinate (%d, %d, %d) marked for foliage"), InCoordinate->X, InCoordinate->Y, InCoordinate->Hierarchy);
		InCoordinate->bIsDirectSurfaceBlock = true;
	}
}

void UDigumWorldGenerator::CheckAndSetNeighbors(FDigumWorldProceduralSection* InSection, const int32& NumOfHierarchies,
                                                FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection,
                                                FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection,
                                                int32 InLocalSectionWidth, int32 InLocalSectionHeight)
{
	if (InSection)
    {
		/*TMap<FName, FDigumWorldProceduralCoordinateArray> CoordinateMap;
		InSection->GetCoordinateArray()->MakeMappedCoordinates(CoordinateMap);
		TArray<FName> BlockIDs;
		CoordinateMap.GetKeys(BlockIDs);*/
		

		for (int32 i = 0; i < NumOfHierarchies; i++)
		{
			const int32 HierarchyIndex = i - (NumOfHierarchies - 1);

			for (int32 j = 0; j < InSection->GetCoordinateArray()->CoordinateCount(); j++)
			{
				const int32 x = j % InLocalSectionWidth;
				const int32 y = j / InLocalSectionWidth;

				FDigumWorldProceduralCoordinate* Coordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y, HierarchyIndex);
				
				if (!Coordinate)
				{
					continue;
				}
				// UE_LOG(LogTemp, Warning, TEXT("Coordinate Hierarchy %i, "), Coordinate->Hierarchy);
				// Reset neighbor flags
				Coordinate->bHasLeftNeighbor = false;
				Coordinate->bHasRightNeighbor = false;
				Coordinate->bHasTopNeighbor = false;
				Coordinate->bHasBottomNeighbor = false;

				// Check left neighbor
				/*if (x > 0)
				{
					FDigumWorldProceduralCoordinate* LeftCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x - 1, y);
					if (LeftCoordinate)
					{
						Coordinate->bHasLeftNeighbor = true;
					}
				}
				else if (InLeftSection)
				{
					FDigumWorldProceduralCoordinate* LeftCoordinate = InLeftSection->GetCoordinateArray()->GetCoordinate(InLocalSectionWidth - 1, y);
					if (LeftCoordinate)
					{
						Coordinate->bHasLeftNeighbor = true;
					}
				}
	
				// Check right neighbor
				if (x < InLocalSectionWidth - 1)
				{
					FDigumWorldProceduralCoordinate* RightCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x + 1, y);
					if (RightCoordinate)
					{
						Coordinate->bHasRightNeighbor = true;
					}
				}
				else if (InRightSection)
				{
					FDigumWorldProceduralCoordinate* RightCoordinate = InRightSection->GetCoordinateArray()->GetCoordinate(0, y);
					if (RightCoordinate)
					{
						Coordinate->bHasRightNeighbor = true;
					}
				}*/

				// Check top neighbor
				if (y > 0)
				{
					FDigumWorldProceduralCoordinate* TopCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y - 1, HierarchyIndex);
					if (TopCoordinate && !TopCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasTopNeighbor = true;
					}
				}
				else if (InTopSection)
				{
					FDigumWorldProceduralCoordinate* TopCoordinate = InTopSection->GetCoordinateArray()->GetCoordinate(x, InLocalSectionHeight - 1, HierarchyIndex);
					if (TopCoordinate && !TopCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasTopNeighbor = true;
					}
					
				}

				// Check bottom neighbor
				if (y < InLocalSectionHeight - 1)
				{
					FDigumWorldProceduralCoordinate* BottomCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y + 1, HierarchyIndex);
					if (BottomCoordinate && !BottomCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasBottomNeighbor = true;
					}
				}
				else if (InBottomSection)
				{
					FDigumWorldProceduralCoordinate* BottomCoordinate = InBottomSection->GetCoordinateArray()->GetCoordinate(x, 0, HierarchyIndex);
					if (BottomCoordinate && !BottomCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasBottomNeighbor = true;
					}
				}

				if(!Coordinate->bHasTopNeighbor && Coordinate->bHasBottomNeighbor && Coordinate->BlockIDs.IsEmpty())
				{
					MarkForFoliage(Coordinate);
				}
			}
		}
    }
}

void UDigumWorldGenerator::GenerateUndergroundVeins(FDigumWorldProceduralMap& OutMap)
{
	
}

float UDigumWorldGenerator::GetGroundNoise(const float& X, const FRandomStream& InRandomStream)
{
	return GetPerlinNoiseValue1D(X, InRandomStream);
}

float UDigumWorldGenerator::QuadraticBezier(float t, float P0, float P1, float P2)
{
	float u = 1 - t;
	return u * u * P0 + 2 * u * t * P1 + t * t * P2;
}

float UDigumWorldGenerator::CubicBezier(float t, float P0, float P1, float P2, float P3)
{
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	return uuu * P0 + 3 * uu * t * P1 + 3 * u * tt * P2 + ttt * P3;
}

TArray<float> UDigumWorldGenerator::SmoothTerrain(const TArray<float>& GroundCurve, int32 SmoothingPasses)
{
	TArray<float> SmoothedCurve = GroundCurve;

	for (int32 pass = 0; pass < SmoothingPasses; ++pass)
	{
		for (int32 x = 1; x < SmoothedCurve.Num() - 1; ++x)
		{
			SmoothedCurve[x] = (GroundCurve[x - 1] + GroundCurve[x] + GroundCurve[x + 1]) / 3.0f;
		}
	}

	return SmoothedCurve;
}

float UDigumWorldGenerator::GetPerlinNoiseValue2D(const float InX, const int32 InY, const FRandomStream& RandomStream) 
{
	constexpr float Scale = 0.1f;
	const FVector2D NoiseInput = FVector2D(InX * Scale, InY * Scale) + FVector2D(RandomStream.GetFraction(), RandomStream.GetFraction());
	return FMath::PerlinNoise2D(NoiseInput);
}

float UDigumWorldGenerator::GetPerlinNoiseValue1D(const float InX, const FRandomStream& InRandomStream)
{
	constexpr float Scale = 0.1f;
	const float NoiseInput = InX * Scale + InRandomStream.GetFraction();
	return FMath::PerlinNoise1D(NoiseInput);
}

float UDigumWorldGenerator::GetPerlinNoiseValue3D(const float InX, const int32 InY, const int32 InZ,
                                                  const FRandomStream& InRandomStream)
{
	constexpr float Scale = 0.1f;
	const FVector RandomStreamFraction = FVector(InRandomStream.GetFraction(), InRandomStream.GetFraction(), InRandomStream.GetFraction() + 100);
	const FVector VectorScale = FVector(InX * Scale, InY * Scale, InZ * Scale);
	const FVector NoiseInput = VectorScale + RandomStreamFraction;
	return FMath::PerlinNoise3D(NoiseInput);
}
