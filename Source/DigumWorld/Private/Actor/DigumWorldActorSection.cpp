// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorSection.h"

#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Component/DigumVisibilityComponent.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldActorSection::ADigumWorldActorSection()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	
	VisibilityComponent = CreateDefaultSubobject<UDigumVisibilityComponent>(TEXT("VisibilityComponent"));
	
}

void ADigumWorldActorSection::OnSetWorldVisibility(bool bValue)
{
	/*for(auto It = WorldChildActors.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->Value)
		{
			/*ChildActor->SetActorHiddenInGame(bValue);
			ChildActor->SetWorldCollision(bValue);#1#
		}
	}*/

}

// Called when the game starts or when spawned
void ADigumWorldActorSection::BeginPlay()
{
	Super::BeginPlay();

	if(VisibilityComponent)
	{
		VisibilityComponent->GetOnSetVisibilityDelegate().AddUObject(this, &ADigumWorldActorSection::OnSetWorldVisibility);
	}
}

// Called every frame
void ADigumWorldActorSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADigumWorldActorSection::InitializeSection(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	SectionData = InSection;
	SectionX = InSection.GetX();
	SectionY = InSection.GetY();
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

	for (auto It = Blocks.CreateConstIterator(); It; ++It)
	{
		FName BlockID = It->Key;
		FDigumWorldProceduralCoordinateArray BlockCoordinates = It->Value;
		
		UDigumWorldSwatchAsset* SwatchAsset = UDigumWorldFunctionHelpers::GetSwatchAsset(BlockID, TEXT("Primary"));

		if(SwatchAsset)
		{
			if(TSubclassOf<ADigumWorldActorChild> ChildClass = SwatchAsset->GetChildActorClass())
			{
				// FTransform ChildTransform(FRotator::ZeroRotator, GetActorLocation());
				ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity);
				if(NewActor)
				{
					NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					// NewActor->SetFolderPath(GetFolderPath());
					NewActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates);
					NewActor->FinishSpawning(FTransform::Identity);
					
					WorldChildActors.FindOrAdd(BlockID, NewActor);
/*#if WITH_EDITOR
					NewActor->SetIsHiddenEdLayer(true);
#endif*/
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Child Actor Class not found for BlockID: %s"), *BlockID.ToString());
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


void ADigumWorldActorSection::AddBlock(const FName& InBlockID, const FVector& InLocation, const int32& WidthOffset,
	const int32& HeightOffset)
{
	// Check if block exists
	// const FVector LocalPos
	ADigumWorldActorChild* ChildActor = WorldChildActors.FindRef(InBlockID);
	FDigumWorldProceduralCoordinateArray CoordinateArray = FDigumWorldProceduralCoordinateArray();
	FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate();
	Coordinate.BlockID = InBlockID;

	const int32 X = InLocation.X / GridSize.X;
	const int32 Y = -(InLocation.Z / GridSize.Z);

	UE_LOG(LogTemp, Warning, TEXT("Add Block %s %d %d"), *InBlockID.ToString(), X, Y);
	
	Coordinate.X = FMath::Abs(X % WidthOffset);
	Coordinate.Y = FMath::Abs(Y) > 0? Y % WidthOffset : 0;
	Coordinate.Hierarchy = 0;
	
	UE_LOG(LogTemp, Warning, TEXT("Add Block %s %d %d"), *InBlockID.ToString(), Coordinate.X, Coordinate.Y);

	CoordinateArray.AddCoordinate(Coordinate);
	
	if(ChildActor)
	{
		ChildActor->AddBlock(CoordinateArray);
	}
	else
	{
		UDigumWorldSwatchAsset* Asset = UDigumWorldFunctionHelpers::GetSwatchAsset(InBlockID, TEXT("Primary"));
		if(Asset)
		{
			if(TSubclassOf<ADigumWorldActorChild> ChildClass = Asset->GetChildActorClass())
			{
				ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity);
				if(NewActor)
				{
					NewActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					// NewActor->SetFolderPath(GetFolderPath());
					NewActor->InitializeSwatchAsset(InBlockID, Asset, CoordinateArray);
					NewActor->FinishSpawning(FTransform::Identity);
					NewActor->SetActorLocation(GetActorLocation());
					
					WorldChildActors.FindOrAdd(InBlockID, NewActor);
				}
			}
		}

	}
	
	// create block if it doesnt exist
}

void ADigumWorldActorSection::DestroySection()
{
	for(auto It = WorldChildActors.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->Value)
		{
			ChildActor->Destroy();
		}
	}

	Destroy();
}

int32 ADigumWorldActorSection::GetX() const
{
	return FMath::FloorToInt(GetActorLocation().X / SectionSize.X);
}

int32 ADigumWorldActorSection::GetY() const
{
	return FMath::FloorToInt(GetActorLocation().Z / SectionSize.Y);
}

