// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorSection.h"

#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Component/DigumVisibilityComponent.h"
#include "Components/BoxComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Interface/IDigumPawnInterface.h"
#include "Interface/IDigumWorldPawnInterface.h"
#include "Net/UnrealNetwork.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldActorSection::ADigumWorldActorSection()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	bReplicates = true;
	
}

void ADigumWorldActorSection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumWorldActorSection, ChildActorsContainers);
	DOREPLIFETIME(ADigumWorldActorSection, SectionData);
	DOREPLIFETIME(ADigumWorldActorSection, GridSize);
	DOREPLIFETIME(ADigumWorldActorSection, SectionSize);
	
}


// Called when the game starts or when spawned
void ADigumWorldActorSection::BeginPlay()
{
	Super::BeginPlay();

}

void ADigumWorldActorSection::CleanupSection()
{
	
	OnSectionReadyForCleanup.Broadcast(this);
	// SetSectionEnabled(false);
}

// Called every frame
void ADigumWorldActorSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ADigumWorldActorSection::Reinitialize()
{
	GetWorld()->GetTimerManager().ClearTimer(CleanupTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CleanupTimerHandle, this, &ADigumWorldActorSection::CleanupSection, CleanupTimer, false);
}

void ADigumWorldActorSection::InitializeSpawnData(const FVector2D& InSectionSize,
	FDigumWorldProceduralSection& InSection)
{

	InitializeSection(InSectionSize, InSection, nullptr);
}

void ADigumWorldActorSection::OnChildTransact(ADigumWorldActorChild* InWorldChild,
	const FDigumWorldProceduralSection& InDigumWorldProceduralSection,
	const FDigumWorldRequestParams& InDigumWorldRequestParams)
{
	
}


void ADigumWorldActorSection::InitializeSection(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{

	SectionData = InSection;

	SectionSize = InSectionSize;
	GridSize = GetDefault<UDigumWorldSettings>()->GridSize;

	TMap<FName, FDigumWorldProceduralCoordinateArray> Blocks;
	FDigumWorldProceduralCoordinateArray* Array = InSection.GetCoordinateArray();
	
	if(!Array)
	{
		UE_LOG(LogTemp, Warning, TEXT("Coordinate Array is null"));
		return;
	}

	Array->MakeMappedCoordinates(Blocks);
	EnableSection();
	for (auto It = Blocks.CreateConstIterator(); It; ++It)
	{
		FName BlockID = It->Key;
		FDigumWorldProceduralCoordinateArray BlockCoordinates = It->Value;
		
		UDigumWorldSwatchAsset* SwatchAsset = UDigumWorldFunctionHelpers::GetSwatchAsset(BlockID, TEXT("Primary"));
		
		if(SwatchAsset)
		{
			if(ADigumWorldActorChild* ChildActor = GetChildActor(BlockID))
			{
				ChildActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates, SectionData.HierarchyCount, SectionData.SectionWidth, SectionData.SectionHeight);
			}
			else
			{
				if(TSubclassOf<ADigumWorldActorChild> ChildClass = SwatchAsset->GetChildActorClass())
				{
					// FTransform ChildTransform(FRotator::ZeroRotator, GetActorLocation());
					ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity);
					if(NewActor)
					{
						NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
						NewActor->SetReplicates(true);
						NewActor->SetSectionData(SectionData);
						NewActor->GetOnDigumWorldTransactDelegate().AddUObject(this, &ADigumWorldActorSection::OnChildTransact);
						// NewActor->InitializeISMComponent()
						// NewActor->SetFolderPath(GetFolderPath());
						NewActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates, SectionData.HierarchyCount, SectionData.SectionWidth, SectionData.SectionHeight);
						NewActor->FinishSpawning(FTransform::Identity);

						ChildActorsContainers.Add(FDigumWorldChildActorsContainer(BlockID, NewActor));
						
			
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Child Actor Class not found for BlockID: %s"), *BlockID.ToString());
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Swatch Asset not found for BlockID: %s"), *BlockID.ToString());
		}
	}
}

void ADigumWorldActorSection::CreateChildActor(FDigumWorldProceduralCoordinateArray& InCoordinates)
{
}


void ADigumWorldActorSection::AddBlock(const FName& InBlockID, const FVector& InLocation)
{
	ADigumWorldActorChild* ChildActor = GetChildActor(InBlockID);//WorldChildActors.FindRef(InBlockID);
	FDigumWorldProceduralCoordinateArray CoordinateArray = FDigumWorldProceduralCoordinateArray();
	FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate();
	Coordinate.BlockID = InBlockID;

	const int32 X = InLocation.X / GridSize.X;
	const int32 Y = -(InLocation.Z / GridSize.Z);
	const int32 Hierarchy = -InLocation.Y / GridSize.Y;
	const int32 WidthOffset = SectionData.SectionWidth;
	const int32 HeightOffset = SectionData.SectionHeight;
	Coordinate.X = FMath::Abs(X % WidthOffset);
	Coordinate.Y = FMath::Abs(Y) > 0? Y % WidthOffset : 0;
	Coordinate.Hierarchy = Hierarchy;

	CoordinateArray.AddCoordinate(Coordinate);
	
	if(ChildActor)
	{
		ChildActor->AddBlock(InBlockID,CoordinateArray);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldActorSection::AddBlock, Spawning New Child, %s"), *InBlockID.ToString());
		UDigumWorldSwatchAsset* Asset = UDigumWorldFunctionHelpers::GetSwatchAsset(InBlockID, TEXT("Primary"));
		if(Asset)
		{
			if(TSubclassOf<ADigumWorldActorChild> ChildClass = Asset->GetChildActorClass())
			{
				FVector HierarchyOffset = FVector(0, Hierarchy * GridSize.Y, 0);
				ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity);
				if(NewActor)
				{
					NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					NewActor->InitializeSwatchAsset(InBlockID, Asset, CoordinateArray, SectionData.HierarchyCount, SectionData.SectionWidth, SectionData.SectionHeight);
					NewActor->FinishSpawning(FTransform::Identity);
					NewActor->SetActorLocation(GetActorLocation() + HierarchyOffset);
					
					ChildActorsContainers.Add(FDigumWorldChildActorsContainer(InBlockID, NewActor));
					UE_LOG(LogTemp, Warning, TEXT("Block Added %s"), *InBlockID.ToString());
				}
			}
		}
	}
	
	// create block if it doesnt exist
}

void ADigumWorldActorSection::AddBlock(const FDigumWorldRequestParams& InRequestParams, const FVector& InWorldOffset)
{
	const FName BlockID = InRequestParams.BlockID;
	const FVector Location = InRequestParams.HitLocation - InWorldOffset;
	
	ADigumWorldActorChild* ChildActor = GetChildActor(BlockID);//WorldChildActors.FindRef(InBlockID);
	FDigumWorldProceduralCoordinateArray CoordinateArray = FDigumWorldProceduralCoordinateArray();
	FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate();
	Coordinate.BlockID = BlockID;

	const int32 X = Location.X / GridSize.X;
	const int32 Y = -(Location.Z / GridSize.Z);
	const int32 Hierarchy = -Location.Y / GridSize.Y;
	const int32 WidthOffset = SectionData.SectionWidth;
	const int32 HeightOffset = SectionData.SectionHeight;
	Coordinate.X = FMath::Abs(X % WidthOffset);
	Coordinate.Y = FMath::Abs(Y) > 0? Y % WidthOffset : 0;
	Coordinate.Hierarchy = Hierarchy;

	CoordinateArray.AddCoordinate(Coordinate);
	
	if(ChildActor)
	{
		ChildActor->AddBlock(InRequestParams, CoordinateArray);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldActorSection::AddBlock, Spawning New Child, %s"), *BlockID.ToString());
		UDigumWorldSwatchAsset* Asset = UDigumWorldFunctionHelpers::GetSwatchAsset(BlockID, TEXT("Primary"));
		if(Asset)
		{
			if(TSubclassOf<ADigumWorldActorChild> ChildClass = Asset->GetChildActorClass())
			{
				FVector HierarchyOffset = FVector(0, Hierarchy * GridSize.Y, 0);
				ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity);
				if(NewActor)
				{
					NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					NewActor->InitializeSwatchAsset_UsingParams(Asset, CoordinateArray, SectionData.HierarchyCount, SectionData.SectionWidth, SectionData.SectionHeight, InRequestParams);
					NewActor->FinishSpawning(FTransform::Identity);
					NewActor->SetActorLocation(GetActorLocation() + HierarchyOffset);
					
					ChildActorsContainers.Add(FDigumWorldChildActorsContainer(BlockID, NewActor));
					UE_LOG(LogTemp, Warning, TEXT("Block Added %s"), *BlockID.ToString());
				}
			}
		}
	}
}


void ADigumWorldActorSection::DestroySection()
{
	for(auto It = ChildActorsContainers.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->ChildActor)
		{
			ChildActor->Destroy();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Destroy Section"));

	Destroy();
}

void ADigumWorldActorSection::ResetSection()
{
	SectionData = FDigumWorldProceduralSection(-1,-1);
	
	for(auto It = ChildActorsContainers.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->ChildActor)
		{
			ChildActor->ResetChildActor();
		}
	}
	
}

void ADigumWorldActorSection::EnableSection()
{
	for(auto It = ChildActorsContainers.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->ChildActor)
		{
			ChildActor->SetActorEnableCollision(true);
			ChildActor->SetActorHiddenInGame(false);
		}
	}
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ADigumWorldActorSection::SetSectionEnabled(const bool& bValue)
{
	const bool bCollisionEnabled = bValue;
	const bool bHiddenInGame = !bValue;
	for(auto It = ChildActorsContainers.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->ChildActor)
		{
			ChildActor->SetActorEnableCollision(bCollisionEnabled);
			ChildActor->SetActorHiddenInGame(bHiddenInGame);
		}
	}

	SetActorEnableCollision(bCollisionEnabled);
	SetActorHiddenInGame(bHiddenInGame);

}

FDigumWorldChildActorsContainer* ADigumWorldActorSection::GetChildActorContainer(const FName& InBlockID)
{
	for(auto& Container: ChildActorsContainers)
	{
		if(Container.BlockID == InBlockID)
		{
			return &Container;
		}
	}

	return nullptr;
}

ADigumWorldActorChild* ADigumWorldActorSection::GetChildActor(const FName& InBlockID)
{
	if(FDigumWorldChildActorsContainer* Container = GetChildActorContainer(InBlockID))
	{
		return Container->ChildActor;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Container is null"));
	return nullptr;
}