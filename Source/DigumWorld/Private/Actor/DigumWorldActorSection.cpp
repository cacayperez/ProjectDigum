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
#include "Procedural/DigumWorldGenerator.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldActorSection::ADigumWorldActorSection()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	/*BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(Root);*/
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

	/*if(BoxComponent)
	{
		GetWorld()->GetTimerManager().SetTimer(ReuseTimerHandle, this, &ADigumWorldActorSection::FlagForReuse, 3.0f, false);
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADigumWorldActorSection::OnBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADigumWorldActorSection::OnEndOverlap);
	}*/
}

void ADigumWorldActorSection::CleanupSection()
{
	OnSectionReadyForCleanup.Broadcast(this);
}

// Called every frame
void ADigumWorldActorSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ADigumWorldActorSection::Reinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Reinitialize Section"));
	GetWorld()->GetTimerManager().ClearTimer(CleanupTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CleanupTimerHandle, this, &ADigumWorldActorSection::CleanupSection, CleanupTimer, false);
}

void ADigumWorldActorSection::InitializeSection(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	GetWorld()->GetTimerManager().SetTimer(CleanupTimerHandle, this, &ADigumWorldActorSection::CleanupSection, CleanupTimer, false);
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
	EnableSection();
	for (auto It = Blocks.CreateConstIterator(); It; ++It)
	{
		FName BlockID = It->Key;
		FDigumWorldProceduralCoordinateArray BlockCoordinates = It->Value;
		
		UDigumWorldSwatchAsset* SwatchAsset = UDigumWorldFunctionHelpers::GetSwatchAsset(BlockID, TEXT("Primary"));
		
		if(SwatchAsset)
		{
			ADigumWorldActorChild* ChildActor = WorldChildActors.FindRef(BlockID);
			if(ChildActor)
			{
				// TODO How to set variants?
				ChildActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates);
				
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
						// NewActor->SetFolderPath(GetFolderPath());
						NewActor->InitializeSwatchAsset(BlockID, SwatchAsset, BlockCoordinates);
						NewActor->FinishSpawning(FTransform::Identity);
					
						WorldChildActors.FindOrAdd(BlockID, NewActor);
						/*#if WITH_EDITOR
											NewActor->SetIsHiddenEdLayer(true);
						#endif*/
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


void ADigumWorldActorSection::AddBlock(const FName& InBlockID, const FVector& InLocation, const int32& WidthOffset,
	const int32& HeightOffset)
{
	ADigumWorldActorChild* ChildActor = WorldChildActors.FindRef(InBlockID);
	FDigumWorldProceduralCoordinateArray CoordinateArray = FDigumWorldProceduralCoordinateArray();
	FDigumWorldProceduralCoordinate Coordinate = FDigumWorldProceduralCoordinate();
	Coordinate.BlockID = InBlockID;

	const int32 X = InLocation.X / GridSize.X;
	const int32 Y = -(InLocation.Z / GridSize.Z);
	const int32 Hierarchy = -InLocation.Y / GridSize.Y;
	
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
					NewActor->InitializeSwatchAsset(InBlockID, Asset, CoordinateArray);
					NewActor->FinishSpawning(FTransform::Identity);
					NewActor->SetActorLocation(GetActorLocation() + HierarchyOffset);
					
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

	UE_LOG(LogTemp, Warning, TEXT("Destroy Section"));

	Destroy();
}

void ADigumWorldActorSection::ResetSection()
{
	SectionData = FDigumWorldProceduralSection(-1,-1);
	
	for(auto It = WorldChildActors.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->Value)
		{
			ChildActor->ResetChildActor();
		}
	}
	
}

void ADigumWorldActorSection::EnableSection()
{
	for(auto It = WorldChildActors.CreateConstIterator(); It; ++It)
	{
		if(ADigumWorldActorChild* ChildActor = It->Value)
		{
			ChildActor->SetActorEnableCollision(true);
			ChildActor->SetActorHiddenInGame(false);
		}
	}
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

/*
int32 ADigumWorldActorSection::GetX() const
{
	return FMath::FloorToInt(GetActorLocation().X / SectionSize.X);
}

int32 ADigumWorldActorSection::GetY() const
{
	return FMath::FloorToInt(GetActorLocation().Z / SectionSize.Y);
}
*/
