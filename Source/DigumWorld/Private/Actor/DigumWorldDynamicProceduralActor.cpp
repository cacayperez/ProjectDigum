// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldDynamicProceduralActor.h"

#include "Actor/DigumWorldActorSection.h"
#include "Components/DigumWorldMapLoaderComponent.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldDynamicProceduralActor::ADigumWorldDynamicProceduralActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// SetActorTickInterval(0.1f);
	bReplicates = true;

	WorldMapLoaderComponent = CreateDefaultSubobject<UDigumWorldMapLoaderComponent>(TEXT("WorldMapLoaderComponent"));
	// WorldMapLoaderComponent->InitializeDynamicProceduralMap(this);
}

void ADigumWorldDynamicProceduralActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ADigumWorldDynamicProceduralActor::BeginPlay()
{
	Super::BeginPlay();

	/*APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(Controller)
	{
		SetOwner(Controller);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Controller"));
	}*/

	// Preinitialize sections
	// InitializeDefaultSections();
	
	UDigumWorldProceduralAsset* Asset = ProceduralRules.GetProceduralAsset();
	// const UDigumWorldSettings* Settings = GetDefault<UDigumWorldSettings>();
	if(Asset)
	{
		const FVector SettingGridSize = UDigumWorldSettings::GetGridSize();
		const FName SeedName = ProceduralRules.Seed;
		const int32 SectionWidth = ProceduralRules.SectionWidth;
		const int32 SectionHeight = ProceduralRules.SectionHeight;
		const int32 Horizontal = ProceduralRules.SectionCount_HorizontalAxis;
		const int32 Vertical = ProceduralRules.SectionCount_VerticalAxis;
		const int32 Hierarchies = ProceduralRules.NumberOfHierarchies;
		// ProceduralAsset = Asset;
		GenerateMap(SeedName, SettingGridSize, SectionWidth, SectionHeight, Horizontal, Vertical, Hierarchies, Asset);
		ApplyWorldOffsetPosition();
		
	}

	 Server_SpawnChunksAtLocation(FVector::ZeroVector, 4);
}

void ADigumWorldDynamicProceduralActor::InitializeDefaultSections()
{
	const FVector SettingGridSize = UDigumWorldSettings::GetGridSize();
	const FName SeedName = ProceduralRules.Seed;
	const int32 SectionWidth = ProceduralRules.SectionWidth;
	const int32 SectionHeight = ProceduralRules.SectionHeight;
	const int32 Horizontal = ProceduralRules.SectionCount_HorizontalAxis;
	const int32 Vertical = ProceduralRules.SectionCount_VerticalAxis;
	const int32 Hierarchies = ProceduralRules.NumberOfHierarchies;

	for(int32 x = 0; x < Horizontal; x++)
	{
		for(int32 y = 0; y < Vertical; y++)
		{
			WorldMapLoaderComponent->RequestSection(x, y);
		}
	}
}

bool ADigumWorldDynamicProceduralActor::AddSection_Internal(FDigumWorldProceduralSection& InSection, const bool& bNew)
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
		// UE_LOG(LogTemp, Warning, TEXT("Section Actor already exists at %i, %i"), SX, SY);
		// Important for resetting cleanup timer
		Section->Reinitialize();
		return true;
	}
	
	const float X = (SX * (UnitSectionSize.X));
	const float Z = -(SY * (UnitSectionSize.Y));
	const FVector SectionLocation = FVector(X, 0, Z) + WorldOffset;

	if(const bool bResult = SpawnSectionFromPool(SectionLocation, FRotator::ZeroRotator, InSection))
	{
		if(bNew)
		{
			SectionDataArray.Add(InSection);
		}

		return bResult;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to add section from pool"));
	}

	return false;
}

void ADigumWorldDynamicProceduralActor::Multicast_HandleCoordinateChanged_Implementation(
	const FDigumWorldPositioningParams& InParams)
{
	HandleCoordinateChanged_Internal(InParams);	
}

void ADigumWorldDynamicProceduralActor::HandleCoordinateChanged_Internal(const FDigumWorldPositioningParams& InParams)
{
	bool bExists = false;
	for(auto& Params : PositioningParamsArray)
	{
		if(Params.PlayerId == InParams.PlayerId)
		{
			bExists = true;
			Params.WorldLocation = InParams.WorldLocation;
			break;
		}
	}

	if(!bExists)
	{
		PositioningParamsArray.Add(InParams);
	}
}

void ADigumWorldDynamicProceduralActor::UpdateCoordinates()
{
	for(auto& Params : PositioningParamsArray)
	{
		FDigumWorldProceduralSectionCoordinate SectionCoordinate = GetSectionCoordinate(Params.WorldLocation);
		const int32 MaxX = GetMap()->SectionCount_HorizontalAxis;
		const int32 MaxY = GetMap()->SectionCount_VerticalAxis;
		const TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRect(SectionCoordinate, 3, 0, MaxX, 0, MaxY);
		
		for(auto& Coordinate : Coordinates)
		{
			if(ADigumWorldActorSection* Section = GetSectionActor(Coordinate.X, Coordinate.Y))
			{
				Section->Reinitialize();
			}
		}
	}
}

void ADigumWorldDynamicProceduralActor::Server_SpawnChunks_Implementation(const int32& InHalfSize)
{
	if(HasAuthority())
	{
		Multicast_SpawnChunks(InHalfSize);
	}
}

void ADigumWorldDynamicProceduralActor::Multicast_SpawnChunks_Implementation(const int32& InHalfSize)
{
	SpawnChunks_Internal(InHalfSize);
}

void ADigumWorldDynamicProceduralActor::SpawnChunks_Internal(const int32& InHalfSize)
{
	SpawnChunks(InHalfSize);
	UpdateCoordinates();
}

void ADigumWorldDynamicProceduralActor::Server_SpawnChunksAtLocation_Implementation(const FVector& InWorldLocation,
	const int32& InHalfSize)
{
	if(HasAuthority())
	{
		Multicast_SpawnChunksAtLocation(InWorldLocation, InHalfSize);
	}
}

void ADigumWorldDynamicProceduralActor::Multicast_SpawnChunksAtLocation_Implementation(const FVector& InWorldLocation,
	const int32& InHalfSize)
{
	SpawnChunksAtLocation_Internal(InWorldLocation, InHalfSize);
}

void ADigumWorldDynamicProceduralActor::SpawnChunksAtLocation_Internal(const FVector& InWorldLocation,
	const int32& InHalfSize)
{
	SpawnChunks(InWorldLocation, InHalfSize);
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
	
	OnGenerateMap(InSeed, InGridSize, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis, InSectionCount_VerticalAxis, InNumberOfHierarchies, InProceduralAsset);
}

void ADigumWorldDynamicProceduralActor::OnGenerateMap(const FName InSeed, const FVector InGridSize,
	const int32 InSectionWidth, const int32 InSectionHeight, const int32 InSectionCount_HorizontalAxis,
	const int32 InSectionCount_VerticalAxis, const int32 InNumberOfHierarchies,
	UDigumWorldProceduralAsset* InProceduralAsset)
{
	if(HasAuthority())
	{
		InitializePool(400, GetFolderPath());
		
		UE_LOG(LogTemp, Warning, TEXT("Generating Map %s"), *GetFolderPath().ToString());
		
	}
}

void ADigumWorldDynamicProceduralActor::Server_HandleCoordinateChanged_Implementation(
	const FDigumWorldPositioningParams& InParams)
{
	if(HasAuthority())
	{
		Multicast_HandleCoordinateChanged(InParams);
	}
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
	const int32& InHalfSize)
{
	const int32 MaxX = GetMap()->SectionCount_HorizontalAxis;
	const int32 MaxY = GetMap()->SectionCount_VerticalAxis;
	const TArray<FDigumWorldProceduralSectionCoordinate> Coordinates = GetSectionCoordinatesInRect(InCoordinate, InHalfSize, 0, MaxX, 0, MaxY);
	// ActiveCoordinates = Coordinates;
	
	for(const FDigumWorldProceduralSectionCoordinate& Coordinate : Coordinates)
	{
		bool bAdded = false;
		for(auto& Section : SectionDataArray)
		{
			if(Section.GetX() == Coordinate.X && Section.GetY() == Coordinate.Y)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Section Already Exists"));
				bAdded = true;
				AddSection_Internal(Section);
			}
		}

		if(!bAdded)
			WorldMapLoaderComponent->RequestSection(Coordinate.X, Coordinate.Y);	
	}
}

void ADigumWorldDynamicProceduralActor::SpawnChunks(const int32& HalfSize)
{
	if(WorldMapLoaderComponent == nullptr) return;

	TArray<FDigumWorldProceduralSectionCoordinate> ActorCoordinates;

	for(auto& Params : PositioningParamsArray)
	{
		FDigumWorldProceduralSectionCoordinate Coordinate = GetSectionCoordinate(Params.WorldLocation);
		ActorCoordinates.Add(Coordinate);
	}

	for(const auto& ActorCoordinate : ActorCoordinates)
	{
		SpawnChunks(ActorCoordinate, HalfSize);
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
			Section->SetOwner(GetOwner());
			Section->SetFolderPath(InFolderPath);
			Section->SetActorHiddenInGame(true);
			Section->SetActorEnableCollision(false);
			Section->FinishSpawning(FTransform::Identity);
			InactiveSectionPool.Add(Section);
		}
	}

}

bool ADigumWorldDynamicProceduralActor::SpawnSectionFromPool(const FVector& InLocation, const FRotator& InRotation,
	FDigumWorldProceduralSection& InSection)
{
	ADigumWorldActorSection* Section = nullptr;

	// UE_LOG(LogTemp, Warning, TEXT("Pool Size, %i"), InactiveSectionPool.Num());
	if(InactiveSectionPool.Num() > 0)
	{
		Section = InactiveSectionPool.Pop();
		// UE_LOG(LogTemp, Warning, TEXT("Pool: Adding from pool, %i, %i"), InSection.GetX(), InSection.GetY());
	}
	else if(SectionActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool: Removing from active, %i, %i"), InSection.GetX(), InSection.GetY());
		Section = SectionActors[0];
		SectionActors.Remove(Section);
	}
	
	if(Section && ProceduralAsset)
	{
		Section->ResetSection();
		Section->SetActorHiddenInGame(false);
		Section->SetActorEnableCollision(false);
		Section->InitializeSection(UnitSectionSize, InSection, ProceduralAsset);
		Section->SetActorLocation(InLocation);

		if(!SectionActors.Contains(Section))
			SectionActors.Add(Section);
		
		// UE_LOG(LogTemp, Warning, TEXT("Pool: Spawned Section, %i, %i"), InSection.GetX(), InSection.GetY());
		return true;
	}

	return false;
}

void ADigumWorldDynamicProceduralActor::HandleCharacterCoordinateChanged(const FDigumWorldPositioningParams& InParams)
{

	Server_HandleCoordinateChanged(InParams);
	// HandleCoordinateChanged_Internal(InParams);

}

void ADigumWorldDynamicProceduralActor::RegisterPositioningComponent(UDigumWorldPositioningComponent* InComponent)
{
	const FDigumWorldMap* WorldMap = GetMap();

	if(WorldMap == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map is null"));
		return;
	}
	
	if(InComponent)
	{
		
		InComponent->InitializePositioningComponent(WorldMap->GridSize, WorldMap->SectionWidth, WorldMap->SectionHeight, WorldMap->GetWorldOffset());
		InComponent->GetOnCoordinateChangedDelegate().AddUObject(this, &ADigumWorldDynamicProceduralActor::HandleCharacterCoordinateChanged);

		OnRegisterPositioningComponent(InComponent);
	}
}

void ADigumWorldDynamicProceduralActor::RemoveSection(ADigumWorldActorSection* InSection)
{
	if(InSection && SectionActors.Contains(InSection))
	{
		// UE_LOG(LogTemp, Warning, TEXT("REMOVINGGG SECTION, %i, %i"), InSection->GetSectionData().GetX(), InSection->GetSectionData().GetY());
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

	UE_LOG(LogTemp, Warning, TEXT("Procedural Asset is null"));

	return nullptr;
}

// Called every frame
void ADigumWorldDynamicProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Server_SpawnChunks(2);
	// SpawnChunks(4);
}


bool ADigumWorldDynamicProceduralActor::TryAddSection(FDigumWorldProceduralSection& InSectionToAdd)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Try Add Section, %i, %i"), InSectionToAdd.GetX(), InSectionToAdd.GetY()));
	// UE_LOG(LogTemp, Warning, TEXT("Try Add Section, %i, %i"), InSectionToAdd.GetX(), InSectionToAdd.GetY());

	bool bNewSection = true;
	for(auto& Section : SectionDataArray)
	{
		if(Section == InSectionToAdd)
		{
			bNewSection = false;
			break;
		}
	}

	return AddSection_Internal(InSectionToAdd, bNewSection);
}

void ADigumWorldDynamicProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - WorldOffset;
	
	// Translate World Position to Section Coordinates
	const FDigumWorldProceduralSectionCoordinate SectionCoordinate = GetSectionCoordinate(InBlockLocation);
	// UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, UnitSectionSize, SectionCoordinate);
	
	// UE_LOG(LogTemp, Warning, TEXT("Section Coordinate %s"), *SectionCoordinate.ToString());
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

