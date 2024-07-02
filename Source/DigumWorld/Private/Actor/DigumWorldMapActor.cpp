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
	
}

void ADigumWorldMapActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADigumWorldMapActor, SectionActors);
}

void ADigumWorldMapActor::OnSectionLoaded(FDigumWorldProceduralSection& DigumWorldProceduralSection)
{
	TrySpawnSection(DigumWorldProceduralSection);
	/*if(HasAuthority())
	{
		SpawnSection(DigumWorldProceduralSection);
	}
	else // SpawnLocally
	{
		
		SpawnSection(DigumWorldProceduralSection);
	}*/
	
}

void ADigumWorldMapActor::OnAllSectionsLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("World Map Actor: All Sections Loaded"));

	OnWorldLoaded.Broadcast();
	// GenerateWorldMap();
}

// Called when the game starts or when spawned
void ADigumWorldMapActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("World Map Actor: Begin Play"));
	const FVector SettingGridSize = UDigumWorldSettings::GetGridSize();
	WorldMap = FDigumWorldMap(ProceduralRules, SettingGridSize);

	const int32 NumberOfSections  = WorldMap.SectionCount_HorizontalAxis * WorldMap.SectionCount_VerticalAxis;
	SectionActors.SetNum(NumberOfSections);
	// UnitSectionSize = WorldMap.GetSectionUnitSize();
	if(SectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Map Actor: Initializing Sections"));
		SectionComponent->GetOnInitializeSectionDelegate().AddUObject(this, &ADigumWorldMapActor::OnInitializeSection);
		SectionComponent->GetOnSectionLoadedEventDelegate().AddUObject(this, &ADigumWorldMapActor::OnSectionLoaded);
		SectionComponent->GetOnAllSectionsLoadedEventDelegate().AddUObject(this, &ADigumWorldMapActor::OnAllSectionsLoaded);
		
		SectionComponent->InitializeSections(ProceduralRules, WorldMap);
		
	}

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
		const FVector SectionLocation = FVector(X, 0, Z) + WorldMap.GetWorldOffset();

		UE_LOG(LogTemp, Warning, TEXT("SPAWNING: Section Location: %s"), *SectionLocation.ToString());
		
		SectionActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SectionActor->GetDigumWorldSectionReadyForCleanupDelegate().AddUObject(this, &ADigumWorldMapActor::OnRemoveSection);
		SectionActor->SetOwner(GetOwner());
		SectionActor->SetFolderPath(GetFolderPath());
		SectionActor->InitializeSpawnData(WorldMap.GetSectionUnitSize(),InSection);
		SectionActor->FinishSpawning(FTransform::Identity);
		SectionActor->SetActorLocation(SectionLocation);
		SectionActor->SetSectionEnabled(true);

		const int32 SectionIndex = SX * SY;
		if(SectionActors.IsValidIndex(SectionIndex))
		{
			SectionActors[SectionIndex] = SectionActor;
		}

		UE_LOG(LogTemp, Warning, TEXT("Section Spawned: %i, %i"), SX, SY);
	}
}

ADigumWorldActorSection* ADigumWorldMapActor::GetSection(const int32 InX, const int32 InY)
{
	const int32 Index = InX * InY;
	if(SectionActors.IsValidIndex(Index))
	{
		return SectionActors[Index];
	}
	return nullptr;
}

void ADigumWorldMapActor::GenerateWorldMap()
{
	for(int32 x = 0; x < ProceduralRules.SectionCount_HorizontalAxis; x++)
	{
		for(int32 y = 0; y < ProceduralRules.SectionCount_VerticalAxis; y++)
		{
			if(FDigumWorldProceduralSection* Section = SectionComponent->GetSection(x * y))
			{
				// Do something with the section
				SpawnSection(*Section);
			}
		}
	}
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

void ADigumWorldMapActor::HandleCharacterCoordinateChanged(
	const FDigumWorldPositioningParams& DigumWorldPositioningParams)
{	
	const FVector OffsetLocation = DigumWorldPositioningParams.WorldLocation - WorldMap.GetWorldOffset();
	FDigumWorldProceduralSectionCoordinate OutCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(OffsetLocation, WorldMap.GetSectionUnitSize(), OutCoordinate);

	TArray<FDigumWorldProceduralSectionCoordinate> SectionCoordinates = GetSectionCoordinatesInRect(OutCoordinate, 2, 0, ProceduralRules.SectionCount_HorizontalAxis, 0, ProceduralRules.SectionCount_VerticalAxis);

	for(const auto& SectionCoordinate: SectionCoordinates)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::HandleCharacterCoordinateChanged"));
		EnableSection(SectionCoordinate.X, SectionCoordinate.Y);
	}
}

void ADigumWorldMapActor::RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent)
{	
	if(InComponent)
	{
		
		InComponent->InitializePositioningComponent(WorldMap.GridSize, WorldMap.SectionWidth, WorldMap.SectionHeight, WorldMap.GetWorldOffset());
		InComponent->GetOnCoordinateChangedDelegate().AddUObject(this, &ADigumWorldMapActor::HandleCharacterCoordinateChanged);

		
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
	if(OwningPlayerController && OwningPlayerController->IsLocalController())
	{
		FDigumWorldProceduralSection Section = FDigumWorldProceduralSection(InSection);
		SpawnSection(Section);
	}

}

void ADigumWorldMapActor::TrySpawnSection(FDigumWorldProceduralSection& InSection)
{
	Server_SpawnSection(InSection);
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
