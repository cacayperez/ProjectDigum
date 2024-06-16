// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldDynamicProceduralActor.h"

#include "Actor/DigumWorldActorSection.h"
#include "Components/DigumWorldMapLoaderComponent.h"
#include "Procedural/DigumWorldGenerator.h"


// Sets default values
ADigumWorldDynamicProceduralActor::ADigumWorldDynamicProceduralActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.2f);
	// bReplicates = true;

	WorldMapLoaderComponent = CreateDefaultSubobject<UDigumWorldMapLoaderComponent>(TEXT("WorldMapLoaderComponent"));
	
	
}

// Called when the game starts or when spawned
void ADigumWorldDynamicProceduralActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ADigumWorldDynamicProceduralActor::InitializeDefaultSections()
{
	
}

bool ADigumWorldDynamicProceduralActor::AddSection_Internal(FDigumWorldProceduralSection& InSection)
{
	const int32 SX = InSection.GetX();
	const int32 SY = InSection.GetY();

	if(SX < 0 || SY < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Section Coordinates"));
		return false;
	}
	if(ADigumWorldActorSection* Section = GetSectionActor(SX, SY))
	{
		UE_LOG(LogTemp, Warning, TEXT("Section Actor already exists at %i, %i"), SX, SY);
		// Important for resetting cleanup timer
		Section->Reinitialize();
		return true;
	}
	
	const float X = (SX * (UnitSectionSize.X));
	const float Z = -(SY * (UnitSectionSize.Y));
	const FVector SectionLocation = FVector(X, 0, Z) + WorldOffset;

	return SpawnSectionFromPool(SectionLocation, FRotator::ZeroRotator, InSection);
}

void ADigumWorldDynamicProceduralActor::GenerateMap(const FName InSeed,  const FVector InGridSize, const int32 InSectionWidth,
                                                    const int32 InSectionHeight, int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis,
                                                    const int32 InNumberOfHierarchies, UDigumWorldProceduralAsset* InProceduralAsset)
{
	Map = FDigumWorldMap(InSeed, InGridSize, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis, InSectionCount_VerticalAxis, InNumberOfHierarchies);
	LocalSectionWidth = InSectionWidth;
	LocalSectionHeight = InSectionHeight;
	GridSize = InGridSize;
	UnitSectionSize = Map.GetSectionUnitSize();
	WorldOffset = Map.GetWorldOffset();
	ProceduralAsset = InProceduralAsset;
	// const int32 SectionCount = Map.SectionCount_HorizontalAxis * Map.SectionCount_VerticalAxis;
	/*for(int32 i = 0; i < SectionCount; i++)
	{
		const int32 x = i % Map.SectionCount_HorizontalAxis;
		const int32 y = i / Map.SectionCount_HorizontalAxis;

		FDigumWorldProceduralSection OutSection;
		
		if(GetSection(x, y, OutSection))
		{
			SectionDataArray.Add(OutSection);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to generate section at %d, %d"), x, y);
		}
	}

	for(int32 i = 0; i < SectionDataArray.Num(); i++)
	{
		const int32 x = i % Map.SectionCount_HorizontalAxis;
		const int32 y = i / Map.SectionCount_HorizontalAxis;

		FDigumWorldProceduralSection* Section = &SectionDataArray[i];
		FDigumWorldProceduralSection* LeftSection = nullptr;
		FDigumWorldProceduralSection* RightSection = nullptr;
		FDigumWorldProceduralSection* TopSection = nullptr;
		FDigumWorldProceduralSection* BottomSection = nullptr;

		if(x > 0)
		{
			LeftSection = &SectionDataArray[i - 1];
		}

		if(x < Map.SectionCount_HorizontalAxis - 1)
		{
			RightSection = &SectionDataArray[i + 1];
		}

		if(y > 0)
		{
			TopSection = &SectionDataArray[i - Map.SectionCount_HorizontalAxis];
		}

		if(y < Map.SectionCount_VerticalAxis - 1)
		{
			BottomSection = &SectionDataArray[i + Map.SectionCount_HorizontalAxis];
		}
		
		CheckAndSetNeighbors(Section, Map.NumberOfHierarchies, LeftSection, RightSection, TopSection, BottomSection, LocalSectionWidth, LocalSectionHeight);
		
	}
	
	TArray<FDigumWorldProceduralBlock> PlacedGrassBlocks;
	TArray<FDigumWorldProceduralBlock> PlacedTreesBlocks;
	
	// Foliage
	UDigumWorldGenerator::GenerateFoliage(Map.Seed, SectionDataArray, ProceduralAsset->ProceduralDefinition, PlacedGrassBlocks);
	// UDigumWorldGenerator::GenerateTrees(Map.Seed, SectionDataArray, ProceduralAsset, PlacedTreesBlocks);*/
	OnGenerateMap(InSeed, InGridSize, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis, InSectionCount_VerticalAxis, InNumberOfHierarchies, InProceduralAsset);
}

void ADigumWorldDynamicProceduralActor::OnGenerateMap(const FName InSeed, const FVector InGridSize,
	const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis,
	const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies,
	UDigumWorldProceduralAsset* InProceduralAsset)
{
	InitializePool(100, GetFolderPath());
	WorldMapLoaderComponent->InitializeDynamicProceduralMap(this);
	
}

TArray<FDigumWorldProceduralSectionCoordinate> ADigumWorldDynamicProceduralActor::GetSectionCoordinatesInRect(
	const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin,
	const int32& XMax, const int32& YMin, const int32& YMax) const
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

void ADigumWorldDynamicProceduralActor::SetActiveLocation(
	const FVector& InLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Active Coordinate"))
	ActiveCoordinate = GetSectionCoordinate(InLocation);
	
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize)
{
	FDigumWorldProceduralSectionCoordinate StartCoordinate = GetSectionCoordinate(InWorldLocation);
	SpawnChunks(StartCoordinate, HalfSize);
	
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FDigumWorldProceduralSectionCoordinate& InCoordinate,
	const int32& HalfSize)
{
	if(WorldMapLoaderComponent == nullptr) return;
	
	ActiveCoordinates.Empty();
	const int32 MaxX = GetMap()->SectionCount_HorizontalAxis;
	const int32 MaxY = GetMap()->SectionCount_VerticalAxis;
	const TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRect(InCoordinate, HalfSize, 0, MaxX, 0, MaxY);
	ActiveCoordinates = Coordinates;
	
	for(const FDigumWorldProceduralSectionCoordinate& Coordinate : ActiveCoordinates)
	{
		bool bAdded = false;
		for(auto& Section : SectionDataArray)
		{
			if(Section.GetX() == Coordinate.X && Section.GetY() == Coordinate.Y)
			{
				UE_LOG(LogTemp, Warning, TEXT("Section Already Exists"));
				bAdded = true;
				AddSection_Internal(Section);
				
			}
		}

		if(!bAdded)
			WorldMapLoaderComponent->RequestSection(Coordinate.X, Coordinate.Y);	
	}

}

void ADigumWorldDynamicProceduralActor::InitializePool(int32 InPoolSize, const FName& InFolderPath)
{
	PoolSize = InPoolSize;
	for(int32 i = 0; i < PoolSize; i++)
	{
		if(ADigumWorldActorSection* Section = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity))
		{
			Section->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Section->GetDigumWorldSectionReadyForCleanupDelegate().AddUObject(this, &ADigumWorldDynamicProceduralActor::RemoveSection);
			Section->SetFolderPath(InFolderPath);
			Section->SetActorHiddenInGame(true);
			// Section->SetActorEnableCollision(false);
			Section->FinishSpawning(FTransform::Identity);
			InactiveSectionPool.Add(Section);
		}
	}
}

bool ADigumWorldDynamicProceduralActor::SpawnSectionFromPool(const FVector& InLocation, const FRotator& InRotation,
	FDigumWorldProceduralSection& InSection)
{
	ADigumWorldActorSection* Section = nullptr;

	if(InactiveSectionPool.Num() > 0)
	{
		Section = InactiveSectionPool.Pop();
		UE_LOG(LogTemp, Warning, TEXT("Pool: Adding from pool, %i, %i"), InSection.GetX(), InSection.GetY());
	}
	else if(SectionActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool: Removing from active, %i, %i"), InSection.GetX(), InSection.GetY());
		Section = SectionActors[0];
		SectionActors.Remove(Section);
	}
	
	if(Section)
	{
		Section->ResetSection();
		Section->InitializeSection(UnitSectionSize, InSection, ProceduralAsset);
		Section->SetActorLocation(InLocation);
		// Section->SetActorHiddenInGame(false);

		if(!SectionActors.Contains(Section))
			SectionActors.Add(Section);
		
		UE_LOG(LogTemp, Warning, TEXT("Pool: Spawned Section, %i, %i"), InSection.GetX(), InSection.GetY());
		return true;
	}

	return false;
}

void ADigumWorldDynamicProceduralActor::RemoveSection(ADigumWorldActorSection* InSection)
{
	if(InSection && SectionActors.Contains(InSection))
	{
		UE_LOG(LogTemp, Warning, TEXT("REMOVINGGG SECTION, %i, %i"), InSection->GetSectionData().GetX(), InSection->GetSectionData().GetY());
		InSection->SetActorHiddenInGame(true);
		InSection->SetActorEnableCollision(false);
		InSection->ResetSection();
		SectionActors.Remove(InSection);
		InactiveSectionPool.Add(InSection);
	}
}

FDigumWorldProceduralDefinition* ADigumWorldDynamicProceduralActor::GetProceduralDefinition() const
{
	if(ProceduralAsset)
	{
		return &ProceduralAsset->ProceduralDefinition;
	}

	return nullptr;
}

// Called every frame
void ADigumWorldDynamicProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnChunks(ActiveCoordinate, 3);
}


bool ADigumWorldDynamicProceduralActor::TryAddSection(FDigumWorldProceduralSection& InSectionToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Try Add Section, %i, %i"), InSectionToAdd.GetX(), InSectionToAdd.GetY());


	if(AddSection_Internal(InSectionToAdd))
	{
		SectionDataArray.Add(InSectionToAdd);
		return true;
	}

	return false;
}

void ADigumWorldDynamicProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - WorldOffset;
	
	// Translate World Position to Section Coordinates
	const FDigumWorldProceduralSectionCoordinate SectionCoordinate = GetSectionCoordinate(InBlockLocation);
	// UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, UnitSectionSize, SectionCoordinate);
	
	UE_LOG(LogTemp, Warning, TEXT("Section Coordinate %s"), *SectionCoordinate.ToString());
	// Loop through all sections and add block
	// TODO: Optimize this, currently looping through all sections...
	SectionActors.FindByPredicate([&](ADigumWorldActorSection*& SectionActor)
	{
		if(SectionActor == nullptr) return false;
		const FDigumWorldProceduralSection SectionData = SectionActor->GetSectionData();
		if(SectionData.GetX() == SectionCoordinate.X && SectionData.GetY() ==  SectionCoordinate.Y)
		{
			SectionActor->AddBlock(InBlockID, LocalPosition, LocalSectionWidth, LocalSectionHeight);
			return true;
		}
        
		return false;
	});
}

