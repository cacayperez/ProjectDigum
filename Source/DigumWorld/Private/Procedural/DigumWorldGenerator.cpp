// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Procedural/DigumWorldGenerator.h"

#include "Asset/DigumWorldProceduralAsset.h"
#include "Algo/MinElement.h"
#include "Algo/MaxElement.h"
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

FName UDigumWorldGenerator::GetBlockIDFromNoiseValue(const float InNoiseValue,
	const TArray<TPair<float, float>>& OutCumulativeWeights, const TArray<FDigumWorldProceduralBlock_Sized>& Blocks)
{
	TArray<FDigumWorldProceduralBlock> BlockArray;
	for(const auto& Block : Blocks)
	{
		BlockArray.Add(Block);
	}

	return GetBlockIDFromNoiseValue(InNoiseValue, OutCumulativeWeights, BlockArray);
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

bool UDigumWorldGenerator::GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock_Sized>& Blocks,
                                                TArray<TPair<float, float>>& OutCumulativeWeights, const FVector2D& Seed)
{
	TArray<FDigumWorldProceduralBlock> BlockArray;
	for(const auto& Block : Blocks)
	{
		BlockArray.Add(Block);
	}

	return GetCumulativeWeights(BlockArray, OutCumulativeWeights, Seed);
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


bool UDigumWorldGenerator::IsAreaAvailable(TArray<FDigumWorldProceduralBlock_Sized> InPlacedBlocks, const int32& InStartX, const int32& InStartY,
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


bool UDigumWorldGenerator::GenerateSection(const FDigumWorldMap& InMap, const int32& InSectionX,
                                           const int32& InSectionY, const UDigumWorldProceduralAsset* ProceduralAsset,
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
	if(ProceduralAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Procedural Asset is null"));
		return false;
	}
	const FRandomStream RandomStream(SeedName);
	const int32 MapWidth = InMap.GetTotalSectionWidth();
	const int32 MapHeight = InMap.GetTotalSectionHeight();

	// UE_LOG(LogTemp, Warning, TEXT("Map Width: %d, Map Height: %d"), MapWidth, MapHeight);
	FDigumWorldProceduralSection TempSection;

	const bool bResult = GenerateSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream,  NumberOfHierarchies, ProceduralAsset, TempSection);
	OutSection =  TempSection;
	return bResult;
}



bool UDigumWorldGenerator::GenerateSection(const int32& InSeed, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralSection& OutSection)
{
	const FVector2D CumulativeSeed = FVector2D(InSectionX, InSectionY);
	const int32 Seed = InSeed;
	const int32 SectionWidth = InRules.SectionWidth;
	const int32 SectionHeight = InRules.SectionHeight;
	const int32 NumberOfHierarchies = InRules.NumberOfHierarchies;
	const UDigumWorldProceduralAsset* ProceduralAsset = InRules.GetProceduralAsset();
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->GetTerrainBlocks();
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

	const bool bResult = GenerateSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream, NumberOfHierarchies, ProceduralAsset, TempSection);
	OutSection =  TempSection;
	return bResult;
}

bool UDigumWorldGenerator::GenerateSection(const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream,
                                           const int32& NumOfHierarchies, const UDigumWorldProceduralAsset* ProceduralAsset, FDigumWorldProceduralSection& OutSection)
{
	if(ProceduralAsset == nullptr) return false;
	
	const TArray<FDigumWorldProceduralBlock> TerrainBlocks = ProceduralAsset->GetTerrainBlocks();
	FVector2D CumulativeSeed(InSectionX, InSectionY);

	TArray<float> NormalizedWeights;
	NormalizeWeights(TerrainBlocks, NormalizedWeights);

	TArray<TPair<float, float>> TerrainCumulativeWeights;
	GetCumulativeWeights(NormalizedWeights, TerrainCumulativeWeights);
	
	/*TArray<TPair<float, float>> TerrainCumulativeWeights;
	const bool bHasTerrainCumulativeWeights = GetCumulativeWeights(TerrainBlocks, TerrainCumulativeWeights, CumulativeSeed);*/

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
					Coordinate.AddBlockID(TerrainBlockID);
				}

				OutSection.AddCoordinate(Coordinate);
				bResult = true; // Set to true if we have at least one coordinate
			}
		}
	}
	
	return bResult;
}



bool UDigumWorldGenerator::GenerateTrees(const FName& InSeedName, TArray<FDigumWorldProceduralSection>& InSectionArray,
	const UDigumWorldProceduralAsset* ProceduralAsset, TArray<FDigumWorldProceduralBlock>& InPlacedBlocks)
{
	const TArray<FDigumWorldProceduralBlock> TreesBlocks = ProceduralAsset->GetTreesBlock();
	
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
	TArray<FDigumWorldProceduralSection>& InSectionArray, const UDigumWorldProceduralAsset* ProceduralAsset,
	TArray<FDigumWorldProceduralBlock>& PlaceBlocks)
{
	const TArray<FDigumWorldProceduralBlock> GrassBlocks = ProceduralAsset->GetGrassBlocks();
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
				Coordinate->AddBlockID(OutBlockID, OutVariant);
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
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->GetTerrainBlocks();
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
			if(GenerateSection(MapWidth,MapHeight, h, v, SectionWidth, SectionHeight, RandomStream, NumberOfHierarchies, ProceduralAsset, OutSection))
			{
				TempMap.AddSection(OutSection);
			}
		}
	}

	OutMap = TempMap;
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
