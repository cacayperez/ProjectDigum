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

FName UDigumWorldGenerator::GetBlockIDFromNoiseValue(const float InNoiseValue, const TArray<TPair<float, float>> CumulativeWeights, const TArray<FDigumWorldProceduralBlock>& Blocks)
{
	FName ResultID = NAME_None;
	
	for(int32 i = 0; i < Blocks.Num(); i++)
	{
		const auto& [StartRange, EndRange] = CumulativeWeights[i];
		if(InNoiseValue >= StartRange && InNoiseValue < EndRange)
		{
			ResultID = Blocks[i].BlockID;
			break;
		}
	}
	
	return ResultID;
}

bool UDigumWorldGenerator::GetCumulativeWeights(const TArray<FDigumWorldProceduralBlock>& Blocks, TArray<TPair<float, float>>& OutCumulativeWeights)
{
	if(Blocks.IsEmpty()) return false;
	FName ResultID = NAME_None;
	int32 BlockCount = Blocks.Num();
	
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



TArray<float> UDigumWorldGenerator::GenerateGroundCurve(const int32& InWidth, const int32& InHeight,
	const int32& SectionX, const FRandomStream& InRandomStream)
{
	TArray<float> GroundCurve;
	GroundCurve.SetNum(InWidth);

	const int32 VerticalCenter = InHeight / 2;
	const float AmplitudeScale = 0.2f; // Adjustable amplitude scale
	const float FrequencyScale = 0.3f; // Adjustable frequency
    const float Amplitude = InHeight / 4 * AmplitudeScale; // Adjustable amplitude
    const float NoiseScale = FrequencyScale; // Adjustable frequency

    // Generate Perlin noise values for the base heights
    for (int32 x = 0; x < InWidth; x++)
    {
        float NoiseValue = GetPerlinNoiseValue1D(static_cast<float>(SectionX * InWidth + x) * NoiseScale, InRandomStream);
        GroundCurve[x] = VerticalCenter + NoiseValue * Amplitude;
    }

    // Determine terrain types
    for (int32 x = 0; x < InWidth; x += 20) // Divide terrain into segments of 10 units
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
    			GroundCurve[x + i] = VerticalCenter + NoiseValue * (Amplitude / 4); // Smaller amplitude
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


bool UDigumWorldGenerator::GenerateSection(const FDigumWorldMap& InMap, const int32& InSectionX,
	const int32& InSectionY, const UDigumWorldProceduralAsset* ProceduralAsset,
	FDigumWorldProceduralSection& OutSection)
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Section %i, %i"), InSectionX, InSectionY);
	const FName SeedName = InMap.Seed;
	const int32 SectionWidth = InMap.SectionWidth;
	const int32 SectionHeight = InMap.SectionHeight;
	const int32 NumberOfHierarchies = InMap.NumberOfHierarchies;
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->GetBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights);

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
	const FRandomStream RandomStream(SeedName);
	const int32 MapWidth = InMap.GetTotalSectionWidth();
	const int32 MapHeight = InMap.GetTotalSectionHeight();

	UE_LOG(LogTemp, Warning, TEXT("Map Width: %d, Map Height: %d"), MapWidth, MapHeight);
	FDigumWorldProceduralSection TempSection;

	const bool bResult = GenerateSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream, Blocks, CumulativeWeights, NumberOfHierarchies, TempSection);
	OutSection =  TempSection;
	return bResult;
}



bool UDigumWorldGenerator::GenerateSection(const int32& InSeed, const int32& InSectionX, const int32& InSectionY, const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralSection& OutSection)
{
	const int32 Seed = InSeed;
	const int32 SectionWidth = InRules.SectionWidth;
	const int32 SectionHeight = InRules.SectionHeight;
	const int32 NumberOfHierarchies = InRules.NumberOfHierarchies;
	const UDigumWorldProceduralAsset* ProceduralAsset = InRules.GetProceduralAsset();
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->GetBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights);
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

	const bool bResult = GenerateSection(MapWidth, MapHeight, InSectionX, InSectionY, SectionWidth, SectionHeight, RandomStream, Blocks, CumulativeWeights, NumberOfHierarchies, TempSection);
	OutSection =  TempSection;
	return bResult;
}

bool UDigumWorldGenerator::GenerateSection(const int32& InMapWidth, const int32& InMapHeight, const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream,
                                           const TArray<FDigumWorldProceduralBlock>& InBlocks, const TArray<TPair<float, float>>& InCumulativeWeights,
                                           const int32& NumOfHierarchies, FDigumWorldProceduralSection& OutSection)
{
	if(InBlocks.IsEmpty()) return false;
	if(InCumulativeWeights.IsEmpty()) return false;
	OutSection = FDigumWorldProceduralSection(InSectionX, InSectionY);
	TArray<float> GroundCurve = GenerateGroundCurve(InWidth, InMapHeight, InSectionX, InRandomStream);
	
	for(int32 i = 0; i < NumOfHierarchies; i++)
	{
		int32 HierchyIndex = i - (NumOfHierarchies -1);
		
		for(int32 x = 0; x < InWidth; x++)
		{
			for(int32 y = 0; y < InHeight; y++)
			{
				float NoiseValue = 0.0f;
				const int32 PositionX = InSectionX * InWidth + x;
				const int32 PositionY = InSectionY * InHeight + y;

				
				
				if(PositionY > GroundCurve[x])
				{
					NoiseValue = GetPerlinNoiseValue3D(PositionX, PositionY, HierchyIndex, InRandomStream);
					const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);

					FName BlockID = GetBlockIDFromNoiseValue(NormalizedNoise, InCumulativeWeights, InBlocks);
					OutSection.AddCoordinate(BlockID, x, y, HierchyIndex, NoiseValue);
				}
				else
				{
					// TODO Do the above ground parts
					//NoiseValue = GetPerlinNoiseValue2D(PositionX, PositionY, InRandomStream);
				}
				
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
	const TArray<FDigumWorldProceduralBlock> Blocks = ProceduralAsset->GetBlocks();
	TArray<TPair<float, float>> CumulativeWeights;
	
	const bool bHasCumulativeWeights = GetCumulativeWeights(Blocks, CumulativeWeights);
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
			if(GenerateSection(MapWidth,MapHeight, h, v, SectionWidth, SectionHeight, RandomStream, Blocks, CumulativeWeights, NumberOfHierarchies, OutSection))
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

void UDigumWorldGenerator::GenerateTrees(FDigumWorldProceduralMap& OutMap)
{
}

void UDigumWorldGenerator::GenerateFoliage(FDigumWorldProceduralMap& OutMap)
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
