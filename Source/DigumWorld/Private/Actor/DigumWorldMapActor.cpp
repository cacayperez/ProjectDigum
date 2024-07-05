// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldMapActor.h"

#include "Actor/DigumWorldActorSection.h"
#include "Components/DigumWorldMapSectionComponent.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Settings/DigumWorldSettings.h"


void ADigumWorldMapActor::OnInitializeSection()
{
}

// Sets default values
ADigumWorldMapActor::ADigumWorldMapActor()
{
	SectionComponent = CreateDefaultSubobject<UDigumWorldMapSectionComponent>(TEXT("SectionComponent"));
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
}

void ADigumWorldMapActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME(ADigumWorldMapActor, SectionActors);
	DOREPLIFETIME(ADigumWorldMapActor, WorldMap);
	DOREPLIFETIME(ADigumWorldMapActor, ProceduralRules);
	DOREPLIFETIME(ADigumWorldMapActor, WorldOffset);*/
}

void ADigumWorldMapActor::OnSectionLoaded(FDigumWorldProceduralSection& DigumWorldProceduralSection)
{
	TrySpawnSection(DigumWorldProceduralSection);
}

void ADigumWorldMapActor::OnAllSectionsLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("World Map Actor: All Sections Loaded"));

	OnWorldLoaded.Broadcast();
}

// Called when the game starts or when spawned
void ADigumWorldMapActor::BeginPlay()
{
	Super::BeginPlay();
	

}

void ADigumWorldMapActor::OnRemoveSection(ADigumWorldActorSection* InDigumWorldActorSection)
{
}

void ADigumWorldMapActor::SpawnSection(FDigumWorldProceduralSection& InSection)
{
	if(ADigumWorldActorSection* SectionActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity))
	{
		const int32 SX = InSection.GetX();
		const int32 SY = InSection.GetY();

		if(SX < 0 || SY < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Section Coordinates"));
			return;
		}
		
		const FVector2D UnitSectionSize = WorldMap.GetSectionUnitSize();
		const float X = (SX * (UnitSectionSize.X));
		const float Z = -(SY * (UnitSectionSize.Y));
		const FVector SectionLocation = FVector(X, 0, Z) + WorldOffset;
		
		SectionActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SectionActor->GetDigumWorldSectionReadyForCleanupDelegate().AddUObject(this, &ADigumWorldMapActor::OnRemoveSection);
		SectionActor->SetOwner(GetOwner());
		SectionActor->SetFolderPath(GetFolderPath());
		SectionActor->InitializeSpawnData(WorldMap.GetSectionUnitSize(),InSection);
		SectionActor->FinishSpawning(FTransform::Identity);
		SectionActor->SetActorLocation(SectionLocation);
		// SectionActor->SetSectionEnabled(true);

		// const int32 ColumnHeight = ;
		
		const int32 SectionIndex = GetSectionIndex(SX, SY);
		// const int32 Index = ;
		if(SectionActors.IsValidIndex(SectionIndex))
		{
			SectionActors[SectionIndex] = SectionActor;
		}

		UE_LOG(LogTemp, Warning, TEXT("Section Spawned: %i, %i, SectionIndex, %i"), SX, SY, SectionIndex);
	}
}

ADigumWorldActorSection* ADigumWorldMapActor::GetSection(const int32 InX, const int32 InY)
{
	const int32 SectionIndex = GetSectionIndex(InX, InY);
	if(SectionActors.IsValidIndex(SectionIndex))
	{
		return SectionActors[SectionIndex];
	}
	return nullptr;
}



void ADigumWorldMapActor::EnableSection(const int32 InX, const int32 InY)
{
	if(ADigumWorldActorSection* SectionActor = GetSection(InX , InY))
	{
		SectionActor->SetSectionEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::EnableSection Section Not Found: %i, %i"), InX, InY);
	
	}
}


void ADigumWorldMapActor::Server_SpawnSection_Implementation(const FDigumWorldProceduralSection& InSection)
{
	if(HasAuthority())
	{
		Multicast_SpawnSection(InSection);
	}
}

void ADigumWorldMapActor::Multicast_SpawnSection_Implementation(const FDigumWorldProceduralSection& InSection)
{
	// if(GetOwner())
	if(HasAuthority())
	{
		FDigumWorldProceduralSection Section = FDigumWorldProceduralSection(InSection);
		SpawnSection(Section);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::Multicast_SpawnSection_Implementation: No Owner"));
	}
}

void ADigumWorldMapActor::TrySpawnSection(FDigumWorldProceduralSection& InSection)
{
	/*if(OwningPlayerController && OwningPlayerController->IsLocalController())
	{
		SpawnSection(InSection);
	}*/
	SpawnSection(InSection);
	// Server_SpawnSection(InSection);
}

void ADigumWorldMapActor::BeginInitializeMap()
{
	UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::BeginInitializeMap Called"));
	
	if(SectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Map Actor: Begin Play"));
		const FVector SettingGridSize = UDigumWorldSettings::GetGridSize();
		WorldMap = FDigumWorldMap();
		WorldMap.GridSize = SettingGridSize;
		WorldMap.SectionWidth = ProceduralRules.SectionWidth;
		WorldMap.SectionHeight = ProceduralRules.SectionHeight;
		WorldMap.SectionCount_HorizontalAxis = ProceduralRules.SectionCount_HorizontalAxis;
		WorldMap.SectionCount_VerticalAxis = ProceduralRules.SectionCount_VerticalAxis;
		WorldMap.NumberOfHierarchies = ProceduralRules.NumberOfHierarchies;
	
		const int32 NumberOfSections  = WorldMap.SectionCount_HorizontalAxis * WorldMap.SectionCount_VerticalAxis;
		SectionActors.SetNum(NumberOfSections);
		SectionComponent->InitializeSections(ProceduralRules, WorldMap);
		if(SectionComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("World Map Actor: Initializing Sections"));
			SectionComponent->GetOnInitializeSectionDelegate().AddUObject(this, &ADigumWorldMapActor::OnInitializeSection);
			SectionComponent->GetOnSectionLoadedEventDelegate().AddUObject(this, &ADigumWorldMapActor::OnSectionLoaded);
			SectionComponent->GetOnAllSectionsLoadedEventDelegate().AddUObject(this, &ADigumWorldMapActor::OnAllSectionsLoaded);
		}
	
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::BeginInitializeMap SectionComponent"));
		WorldOffset =  WorldMap.GetWorldOffset();
		SetActorLocation(WorldOffset);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::BeginInitializeMap: Section Component is NULL"));
	
	}

}

void ADigumWorldMapActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - WorldOffset;
	
	// Translate World Position to Section Coordinates
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, WorldMap.GetSectionUnitSize(), SectionCoordinate);

	if(SectionCoordinate.IsValid())
	{
		const int32 SectionIndex = GetSectionIndex(SectionCoordinate.X, SectionCoordinate.Y);
		
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::AddBlock: Valid Section Coordinate: %s, %i"), *SectionCoordinate.ToString(), SectionIndex);
		if(SectionActors.IsValidIndex(SectionIndex))
		{
			if(ADigumWorldActorSection* SectionActor = SectionActors[SectionIndex])
			{
				SectionActor->AddBlock(InBlockID, LocalPosition, WorldMap.SectionWidth, WorldMap.SectionHeight);
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::AddBlock Invalid Section Coordinate"));
	}
}

TArray<FDigumWorldProceduralSectionCoordinate> ADigumWorldMapActor::GetSectionCoordinatesInRect(
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

int32 ADigumWorldMapActor::GetSectionIndex(const int32 InX, const int32 InY) const
{
	return (WorldMap.SectionCount_HorizontalAxis * InY) + InX;
}
