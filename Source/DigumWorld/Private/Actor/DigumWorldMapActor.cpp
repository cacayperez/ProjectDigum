// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldMapActor.h"

#include "Actor/DigumWorldActorChild.h"
#include "Actor/DigumWorldActorSection.h"
#include "Components/DigumWorldMapSectionComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Settings/DigumWorldSettings.h"
#include "Subsystem/DigumWorldSubsystem.h"


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

	DOREPLIFETIME(ADigumWorldMapActor, SectionActors);
	DOREPLIFETIME(ADigumWorldMapActor, WorldMap);
	DOREPLIFETIME(ADigumWorldMapActor, ProceduralRules);
	DOREPLIFETIME(ADigumWorldMapActor, WorldOffset);
	DOREPLIFETIME(ADigumWorldMapActor, bInitializedMap);
}

void ADigumWorldMapActor::OnSectionLoaded(FDigumWorldProceduralSection& DigumWorldProceduralSection)
{
	UE_LOG(LogTemp, Warning, TEXT("Section Loaded"));
	// TrySpawnSection(DigumWorldProceduralSection);
	SpawnSection(DigumWorldProceduralSection);
}

void ADigumWorldMapActor::OnAllSectionsLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("World Map Actor: All Sections Loaded"));

	OnWorldLoaded.Broadcast();
}

void ADigumWorldMapActor::OnWorldRequest(const FDigumWorldRequestParams& DigumWorldRequestParams)
{
	TryExecuteAction(DigumWorldRequestParams);
}

bool ADigumWorldMapActor::IsLocallyOwned()
{
	if(GetOwner())
	{
		// Never local on dedicated server. IsServerOnly() is checked at compile time and optimized out appropriately.
		if (FPlatformProperties::IsServerOnly())
		{
			checkSlow(!bIsLocallyOwned);
			return false;
		}
	
		// Fast path if we have this bool set.
		if (bIsLocallyOwned)
		{
			return true;
		}

		ENetMode NetMode = GetNetMode();
		if (NetMode == NM_DedicatedServer)
		{
			// This is still checked for the PIE case, which would not be caught in the IsServerOnly() check above.
			checkSlow(!bIsLocallyOwned);
			return false;
		}

		if (NetMode == NM_Client || NetMode == NM_Standalone)
		{
			// Clients or Standalone only receive their own PC. We are not ROLE_AutonomousProxy until after PostInitializeComponents so we can't check that.
			bIsLocallyOwned = true;
			return true;
		}

	}
	
	return false;
}

// Called when the game starts or when spawned
void ADigumWorldMapActor::BeginPlay()
{
	Super::BeginPlay();

	BeginInitializeMap();
	OwningPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if(UDigumWorldSubsystem* WorldSubsystem = UDigumWorldSubsystem::Get(GetWorld()))
	{
		WorldSubsystem->GetOnWorldRequestDelegate().AddUObject(this, &ADigumWorldMapActor::OnWorldRequest);
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
		const FVector SectionLocation = FVector(X, 0, Z) + WorldOffset;
		
		SectionActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SectionActor->GetDigumWorldSectionReadyForCleanupDelegate().AddUObject(this, &ADigumWorldMapActor::OnRemoveSection);
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

		// UE_LOG(LogTemp, Warning, TEXT("Section Spawned: %i, %i, SectionIndex, %i"), SX, SY, SectionIndex);
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


void ADigumWorldMapActor::Server_RemoveBlock_Implementation(const FVector& InWorldLocation, const float& InScaledDamage)
{
	if(HasAuthority())
	{
		Multicast_RemoveBlock(InWorldLocation, InScaledDamage);
	}
}

void ADigumWorldMapActor::Multicast_RemoveBlock_Implementation(const FVector& InWorldLocation,
	const float& InScaledDamage)
{
	ENetMode NetMode = GetNetMode();

	if(NetMode == NM_Client || NetMode == NM_Standalone)
	{
		RemoveBlock_Internal(InWorldLocation, InScaledDamage);
	}

	if(NetMode == NM_DedicatedServer  || NetMode == NM_ListenServer)
	{
		RemoveBlock_Internal(InWorldLocation, InScaledDamage);
	}
}

void ADigumWorldMapActor::Multicast_AddBlock_UsingParams_Implementation(const FDigumWorldRequestParams& InParams)
{
	ENetMode NetMode = GetNetMode();

	if(NetMode == NM_Client || NetMode == NM_Standalone)
	{
		AddBlock_Internal_UsingParams(InParams);
	}

	if(NetMode == NM_DedicatedServer  || NetMode == NM_ListenServer)
	{
		AddBlock_Internal_UsingParams(InParams);
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


void ADigumWorldMapActor::Server_SpawnSection_Implementation(const FDigumWorldProceduralSection& InSection)
{
	if(HasAuthority())
	{
		Multicast_SpawnSection(InSection);
	}
}

void ADigumWorldMapActor::Multicast_SpawnSection_Implementation(const FDigumWorldProceduralSection& InSection)
{
	FDigumWorldProceduralSection Section = FDigumWorldProceduralSection(InSection);
	SpawnSection(Section);

}

void ADigumWorldMapActor::TrySpawnSection(FDigumWorldProceduralSection& InSection)
{
	Server_SpawnSection(InSection);
}

void ADigumWorldMapActor::TryExecuteAction(const FDigumWorldRequestParams& InParams)
{
	FVector HitLocation = InParams.HitLocation - WorldOffset;
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(HitLocation, WorldMap.GetSectionUnitSize(), SectionCoordinate);

	if(InParams.Request == EDigumWorld_Request::DigumWorldRequest_Add)
	{
		TryAddBlock(InParams.BlockID, InParams.HitLocation);

	}

	UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::TryExecuteAction, %s"), *InParams.HitLocation.ToString());
}

void ADigumWorldMapActor::BeginInitializeMap()
{
	if(bInitializedMap) return;
	
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

		bInitializedMap = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::BeginInitializeMap: Section Component is NULL"));
	
	}

}

void ADigumWorldMapActor::TryAddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{

	Server_AddBlock(InBlockID, InBlockLocation);
}

void ADigumWorldMapActor::TryAddBlock_UsingParams(const FDigumWorldRequestParams& InParams)
{
	Server_AddBlock_UsingParams(InParams);
}

void ADigumWorldMapActor::TryRemoveBlock(const FVector& InWorldLocation, const float& InScaledDamage)
{
	Server_RemoveBlock(InWorldLocation, InScaledDamage);
}

void ADigumWorldMapActor::Editor_GenerateWorldMap()
{
	BeginInitializeMap();
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

void ADigumWorldMapActor::AddBlock_Internal(const FName& InBlockID, const FVector& InWorldLocation)
{
	// Get Local Position
	const FVector LocalPosition = InWorldLocation - WorldOffset;
	UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::AddBlock: %s"), *LocalPosition.ToString());
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
				SectionActor->AddBlock(InBlockID, LocalPosition);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::AddBlock Invalid Section Coordinate"));
	}
}

void ADigumWorldMapActor::AddBlock_Internal_UsingParams(const FDigumWorldRequestParams& InParams)
{
	const FVector HitLocation = InParams.HitLocation - WorldOffset;
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(HitLocation, WorldMap.GetSectionUnitSize(), SectionCoordinate);

	if(SectionCoordinate.IsValid())
	{
		const int32 SectionIndex = GetSectionIndex(SectionCoordinate.X, SectionCoordinate.Y);
		
		if(SectionActors.IsValidIndex(SectionIndex))
		{
			if(ADigumWorldActorSection* SectionActor = SectionActors[SectionIndex])
			{
				SectionActor->AddBlock(InParams, WorldOffset);
				UE_LOG(LogTemp, Warning, TEXT("Are you working? #2"));
				UE_LOG(LogTemp, Warning, TEXT("#2 Params, %s"), *InParams.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::AddBlock Invalid Section Coordinate"));
	}
}


void ADigumWorldMapActor::Server_AddBlock_Implementation(const FName& InBlockID, const FVector& InWorldLocation)
{
	if(HasAuthority())
	{
		Multicast_AddBlock(InBlockID, InWorldLocation);
	}
}

void ADigumWorldMapActor::Multicast_AddBlock_Implementation(const FName& InBlockID, const FVector& InWorldLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::Multicast_AddBlock_Implementation, %s"), *InWorldLocation.ToString());

	ENetMode NetMode = GetNetMode();

	if(NetMode == NM_Client || NetMode == NM_Standalone)
	{
		AddBlock_Internal(InBlockID, InWorldLocation);
	}

	if(NetMode == NM_DedicatedServer  || NetMode == NM_ListenServer)
	{
		AddBlock_Internal(InBlockID, InWorldLocation);
	}
	
}


void ADigumWorldMapActor::RemoveBlock_Internal(const FVector& InWorldLocation, const float& InScaledDamage)
{
	const FVector Start = InWorldLocation;
	// HACK : Adding +1.0f due to some weird behavior with the box trace
	// identical Start and End locations will not return any hits and always returns FVector::ZeroVector
	const FVector End = InWorldLocation + 1.0f; 
	
	// TArray<FHitResult> OutHitResult;
	FHitResult OutHitResult;
	TArray<AActor*> IgnoredActors;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20.0f, UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if(bHit)
	{
		AActor* HitActor = OutHitResult.GetActor();
		FVector ImpactLocation = OutHitResult.ImpactPoint;
		const int32 Index = OutHitResult.Item;
		if(HitActor)
		{
			if(ADigumWorldActorChild* Child = Cast<ADigumWorldActorChild>(HitActor))
			{
				Child->RemoveBlock(Index, InScaledDamage);
			}
		}
	}
}

void ADigumWorldMapActor::Server_AddBlock_UsingParams_Implementation(const FDigumWorldRequestParams& InParams)
{
	if(HasAuthority())
	{
		Multicast_AddBlock_UsingParams(InParams);
	}
}


