// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorSection.h"

#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldActorSection::ADigumWorldActorSection()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ADigumWorldActorSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumWorldActorSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADigumWorldActorSection::InitializeSection(FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	GridSize = GetDefault<UDigumWorldSettings>()->GridSize;
	TMap<FName, FDigumWorldProceduralCoordinateArray> Blocks;
	FDigumWorldProceduralCoordinateArray* Array = InSection.GetCoordinateArray();
	if(!Array)
	{
		return;
	}
	SectionData = InSection;
	SectionX = InSection.GetX();
	SectionY = InSection.GetY();
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
				FTransform ChildTransform(FRotator::ZeroRotator, GetActorLocation());
				ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, ChildTransform);
				if(NewActor)
				{
					NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
					// NewActor->SetFolderPath(GetFolderPath());
					NewActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates);
					NewActor->FinishSpawning(ChildTransform);
					
					WorldChildActors.FindOrAdd(BlockID, NewActor);
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
	// Check if block exists
	ADigumWorldActorChild* ChildActor = WorldChildActors.FindRef(InBlockID);
	FDigumWorldProceduralCoordinateArray CoordinateArray = FDigumWorldProceduralCoordinateArray();
	FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate();
	Coordinate.X = 0;
	Coordinate.Y = 2;
	Coordinate.Hierarchy = 2;

	CoordinateArray.AddCoordinate(Coordinate);
	
	if(ChildActor)
	{
		ChildActor->AddBlock(CoordinateArray);
	}
	else
	{
		
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

