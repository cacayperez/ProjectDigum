// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldDynamicProceduralActor.h"

#include "Actor/DigumWorldActorSection.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Object/DigumActorPool.h"
#include "Procedural/DigumWorldGenerator.h"


// Sets default values
ADigumWorldDynamicProceduralActor::ADigumWorldDynamicProceduralActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// bReplicates = true;
	
}

// Called when the game starts or when spawned
void ADigumWorldDynamicProceduralActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ADigumWorldDynamicProceduralActor::InitializeDefaultSections()
{
	
}

void ADigumWorldDynamicProceduralActor::OnGenerateMap(const FName InSeed, const FVector InGridSize,
	const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis,
	const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies,
	UDigumWorldProceduralAsset* InProceduralAsset)
{
	Super::OnGenerateMap(InSeed, InGridSize, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis,
	                     InSectionCount_VerticalAxis, InNumberOfHierarchies, InProceduralAsset);

	InitializePool(100, GetFolderPath());
}

ADigumWorldActorSection* ADigumWorldDynamicProceduralActor::GetSectionActor(const int32& InX, const int32& InY) const
{
	for(auto& Section : ActiveSectionPool)
	{
		if(Section->GetSectionData().SectionCoordinate.IsValid())
		{
			if(Section->GetSectionData().GetX() == InX && Section->GetSectionData().GetY() == InY)
			{
				return Section;
			}
		}
	}
	return nullptr;
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

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FVector& InWorldLocation, const int32& HalfSize)
{
	FDigumWorldProceduralSectionCoordinate StartCoordinate = GetSectionCoordinate(InWorldLocation);
	SpawnChunks(StartCoordinate, HalfSize);
	
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const FDigumWorldProceduralSectionCoordinate& InCoordinate,
	const int32& HalfSize)
{
	ActiveCoordinates.Empty();
	const int32 MaxX = GetMap()->SectionCount_HorizontalAxis;
	const int32 MaxY = GetMap()->SectionCount_VerticalAxis;
	const TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRect(InCoordinate, HalfSize, 0, MaxX, 0, MaxY);
	ActiveCoordinates = Coordinates;

	
	for(const FDigumWorldProceduralSectionCoordinate& Coordinate : ActiveCoordinates)
	{
		UE_LOG(LogTemp, Warning, TEXT("Coordinate: %s"), *Coordinate.ToString())
		for(auto& SectionItem: SectionDataArray)
		{
			if(SectionItem.GetX() == Coordinate.X-1 && SectionItem.GetY() == Coordinate.Y)
			{
				CreateSection(SectionItem);
			}
		}
	}

	PurgeChunks();
}

void ADigumWorldDynamicProceduralActor::PurgeChunks()
{
	TArray<ADigumWorldActorSection*> Array;
	for(auto Coordinate : ActiveCoordinates)
	{
		if(ADigumWorldActorSection* Actor = GetSectionActor(Coordinate.X, Coordinate.Y))
		{
			Array.Add(Actor);
		}
	}

	for(auto Actor : ActiveSectionPool)
	{
		if(Actor && !Array.Contains(Actor))
		{
			DespawnActorFromPool(Actor);

		}
	}

	if(InactiveSectionPool.Num() > PoolSize)
	{
		const int32 Start = PoolSize -1;
		const int32 Excess = InactiveSectionPool.Num() - PoolSize;

		InactiveSectionPool.RemoveAt(Start, Excess, true);
	}
}

void ADigumWorldDynamicProceduralActor::OnSectionSpawned(AActor* Actor)
{
	if(Actor)
	{
		
	}
}

void ADigumWorldDynamicProceduralActor::InitializePool(int32 InPoolSize, const FName& InFolderPath)
{
	PoolSize = InPoolSize;
	for(int32 i = 0; i < PoolSize; i++)
	{
		/*ADigumWorldActorSection* Section = GetWorld()->SpawnActor<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass());
		Section->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Section->SetFolderPath(InFolderPath);
		Section->SetActorHiddenInGame(true);
		Section->SetActorEnableCollision(false);*/
		if(ADigumWorldActorSection* Section = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity))
		{
			Section->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
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
	}
	else if(ActiveSectionPool.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reusing Active Section Pool"));
		Section = ActiveSectionPool[0];
		/*Section->ResetSection();
		ActiveSectionPool.RemoveAt(0);*/
	}
	/*else
	{
		if(ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity))
		{
			NewSection->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			NewSection->SetFolderPath(GetFolderPath());
			NewSection->SetActorHiddenInGame(true);
			NewSection->SetActorEnableCollision(false);
			NewSection->FinishSpawning(FTransform::Identity);
			
			Section = NewSection;
		}
		
	}*/


	if(Section)
	{
		Section->ResetSection();
		Section->InitializeSection(UnitSectionSize, InSection, ProceduralAsset);
		Section->SetActorLocation(InLocation);
		Section->SetActorHiddenInGame(false);
		// Section->SetActorEnableCollision(true);
		ActiveSectionPool.Add(Section);

		UE_LOG(LogTemp, Warning, TEXT("Spawned Section"));
		return true;
	}

	return false;
}

void ADigumWorldDynamicProceduralActor::DespawnActorFromPool(ADigumWorldActorSection* InSection)
{
	// InSection->SetA
	InSection->SetActorHiddenInGame(true);
	// InSection->SetActorEnableCollision(false);
	InSection->ResetSection();

	ActiveSectionPool.Remove(InSection);
	InactiveSectionPool.Add(InSection);
	// InSection->DestroySection();
}

// Called every frame
void ADigumWorldDynamicProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADigumWorldDynamicProceduralActor::CreateSection(FDigumWorldProceduralSection InSection)
{
	// Super::CreateSection(InSection);
	
	const int32 SX = InSection.GetX();
	const int32 SY = InSection.GetY();

	if(SX < 0 || SY < 0) return;
	if(GetSectionActor(SX, SY)) return;
	
	const float X = (SX * (UnitSectionSize.X));
	const float Z = -(SY * (UnitSectionSize.Y));

	UE_LOG(LogTemp, Warning, TEXT("UnitSize %s"), *UnitSectionSize.ToString());
	const FVector SectionLocation = FVector(X, 0, Z) + WorldOffset;

	SpawnSectionFromPool(SectionLocation, FRotator::ZeroRotator, InSection);
	
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
	ActiveSectionPool.FindByPredicate([&](ADigumWorldActorSection*& SectionActor)
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

