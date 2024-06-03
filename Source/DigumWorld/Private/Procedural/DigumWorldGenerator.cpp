// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Procedural/DigumWorldGenerator.h"

#include "Asset/DigumWorldProceduralAsset.h"


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

bool UDigumWorldGenerator::GenerateSection(const int32& InSectionX, const int32& InSectionY, const int32& InWidth, const int32& InHeight, const FRandomStream& InRandomStream,
	const TArray<FDigumWorldProceduralBlock>& InBlocks, const TArray<TPair<float, float>>& InCumulativeWeights,
	FDigumWorldProceduralSection& OutSection, const int32& NumOfHierarchies)
{
	if(InBlocks.IsEmpty()) return false;
	if(InCumulativeWeights.IsEmpty()) return false;

	for(int32 i = 0; i < NumOfHierarchies; i++)
	{
		int32 HierchyIndex = i - (NumOfHierarchies -1);
		
		for(int32 x = 0; x < InWidth; x++)
		{
			for(int32 y = 0; y < InHeight; y++)
			{
				const int32 PositionX = InSectionX * InWidth + x;
				const int32 PositionY = InSectionY * InHeight + y;
				const float NoiseValue = GetPerlinNoiseValue3D(PositionX, PositionY, HierchyIndex, InRandomStream);
				const float NormalizedNoise = NormalizeNoiseValue(NoiseValue);

				FName BlockID = GetBlockIDFromNoiseValue(NormalizedNoise, InCumulativeWeights, InBlocks);
				OutSection.AddCoordinate(BlockID, x, y, HierchyIndex, NoiseValue);
			}
		}
	}
	return true;
}

void UDigumWorldGenerator::GenerateWorldMap(const FDigumWorldProceduralRules& InRules, FDigumWorldProceduralMap& OutMap)
{
	const int32 Seed = InRules.Seed;
	const int32 Width = InRules.SectionWidth;
	const int32 Height = InRules.SectionHeight;
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
	
	for(int32 h = -InRules.SectionCount_HorizontalAxis; h < InRules.SectionCount_HorizontalAxis; h++)
	{
		for(int32 v = 0; v < InRules.SectionCount_VerticalAxis; v++)
		{
			FDigumWorldProceduralSection OutSection = FDigumWorldProceduralSection(h, v);
			if(GenerateSection(h, v, Width, Height, RandomStream, Blocks, CumulativeWeights, OutSection, NumberOfHierarchies))
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

float UDigumWorldGenerator::GetPerlinNoiseValue(const int32 InX, const int32 InY, const FRandomStream& RandomStream) 
{
	constexpr  float Scale = 0.1f;
	const FVector2D NoiseInput = FVector2D(InX * Scale, InY * Scale) + FVector2D(RandomStream.GetFraction(), RandomStream.GetFraction());
	return FMath::PerlinNoise2D(NoiseInput);
}

float UDigumWorldGenerator::GetPerlinNoiseValue3D(const int32 InX, const int32 InY, const int32 InZ,
	const FRandomStream& InRandomStream)
{
	constexpr float Scale = 0.1f;
	const FVector RandomStreamFraction = FVector(InRandomStream.GetFraction(), InRandomStream.GetFraction(), InRandomStream.GetFraction());
	const FVector VectorScale = FVector(InX * Scale, InY * Scale, InZ * Scale);
	const FVector NoiseInput = VectorScale + RandomStreamFraction;
	return FMath::PerlinNoise3D(NoiseInput);
}
