// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Actor/DigumWorldActorSection.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldProceduralActor::ADigumWorldProceduralActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	bReplicates = true;
}

void ADigumWorldProceduralActor::CheckAndSetNeighbors(FDigumWorldProceduralSection* InSection, const int32& NumOfHierarchies, FDigumWorldProceduralSection* InLeftSection, FDigumWorldProceduralSection* InRightSection, FDigumWorldProceduralSection* InTopSection, FDigumWorldProceduralSection* InBottomSection, int32 InLocalSectionWidth, int32 InLocalSectionHeight)
{
	if (InSection)
    {
		/*TMap<FName, FDigumWorldProceduralCoordinateArray> CoordinateMap;
		InSection->GetCoordinateArray()->MakeMappedCoordinates(CoordinateMap);
		TArray<FName> BlockIDs;
		CoordinateMap.GetKeys(BlockIDs);*/
		

		for (int32 i = 0; i < NumOfHierarchies; i++)
		{
			const int32 HierarchyIndex = i - (NumOfHierarchies - 1);

			for (int32 j = 0; j < InSection->GetCoordinateArray()->CoordinateCount(); j++)
			{
				const int32 x = j % InLocalSectionWidth;
				const int32 y = j / InLocalSectionWidth;

				FDigumWorldProceduralCoordinate* Coordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y, HierarchyIndex);
				
				if (!Coordinate)
				{
					continue;
				}
				// UE_LOG(LogTemp, Warning, TEXT("Coordinate Hierarchy %i, "), Coordinate->Hierarchy);
				// Reset neighbor flags
				Coordinate->bHasLeftNeighbor = false;
				Coordinate->bHasRightNeighbor = false;
				Coordinate->bHasTopNeighbor = false;
				Coordinate->bHasBottomNeighbor = false;

				// Check left neighbor
				/*if (x > 0)
				{
					FDigumWorldProceduralCoordinate* LeftCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x - 1, y);
					if (LeftCoordinate)
					{
						Coordinate->bHasLeftNeighbor = true;
					}
				}
				else if (InLeftSection)
				{
					FDigumWorldProceduralCoordinate* LeftCoordinate = InLeftSection->GetCoordinateArray()->GetCoordinate(InLocalSectionWidth - 1, y);
					if (LeftCoordinate)
					{
						Coordinate->bHasLeftNeighbor = true;
					}
				}
	
				// Check right neighbor
				if (x < InLocalSectionWidth - 1)
				{
					FDigumWorldProceduralCoordinate* RightCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x + 1, y);
					if (RightCoordinate)
					{
						Coordinate->bHasRightNeighbor = true;
					}
				}
				else if (InRightSection)
				{
					FDigumWorldProceduralCoordinate* RightCoordinate = InRightSection->GetCoordinateArray()->GetCoordinate(0, y);
					if (RightCoordinate)
					{
						Coordinate->bHasRightNeighbor = true;
					}
				}*/

				// Check top neighbor
				if (y > 0)
				{
					FDigumWorldProceduralCoordinate* TopCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y - 1, HierarchyIndex);
					if (TopCoordinate && !TopCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasTopNeighbor = true;
					}
				}
				else if (InTopSection)
				{
					FDigumWorldProceduralCoordinate* TopCoordinate = InTopSection->GetCoordinateArray()->GetCoordinate(x, InLocalSectionHeight - 1, HierarchyIndex);
					if (TopCoordinate && !TopCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasTopNeighbor = true;
					}
					
				}

				// Check bottom neighbor
				if (y < InLocalSectionHeight - 1)
				{
					FDigumWorldProceduralCoordinate* BottomCoordinate = InSection->GetCoordinateArray()->GetCoordinate(x, y + 1, HierarchyIndex);
					if (BottomCoordinate && !BottomCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasBottomNeighbor = true;
					}
				}
				else if (InBottomSection)
				{
					FDigumWorldProceduralCoordinate* BottomCoordinate = InBottomSection->GetCoordinateArray()->GetCoordinate(x, 0, HierarchyIndex);
					if (BottomCoordinate && !BottomCoordinate->BlockIDs.IsEmpty())
					{
						Coordinate->bHasBottomNeighbor = true;
					}
				}

				if(!Coordinate->bHasTopNeighbor && Coordinate->bHasBottomNeighbor && Coordinate->BlockIDs.IsEmpty())
				{
					MarkForFoliage(Coordinate);
				}
			}
		}
    }
}

void ADigumWorldProceduralActor::MarkForFoliage(FDigumWorldProceduralCoordinate* InCoordinate)
{
	if(InCoordinate)
	{
		
		InCoordinate->bIsDirectSurfaceBlock = true;
	}
}


/*FDigumWorldProceduralSection& ADigumWorldProceduralActor::GetSectionData(const int32& InX, const int32& InY)
{
	/*for(auto& Section : SectionDataArray)
	{
		if(Section.GetX() == InX && Section.GetY() == InY )
		{
			return Section;
		}
	}#1#

	/*return nullptr;#1#
}*/

// Called when the game starts or when spawned
void ADigumWorldProceduralActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumWorldProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADigumWorldProceduralActor::GetSection(const int32& InSectionX, const int32& InSectionY,
	FDigumWorldProceduralSection& OutSection) const
{
	if(ProceduralAsset == nullptr) return false;

	return UDigumWorldGenerator::GenerateTerrainSection(Map, InSectionX, InSectionY, ProceduralAsset->ProceduralDefinition, OutSection);
}


void ADigumWorldProceduralActor::CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, 
                                               FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* InProceduralAsset)
{
	const int32 SX = InSection.GetX();
	const int32 SY = InSection.GetY();

	if(SX < 0 || SY < 0) return;
	if(GetSectionActor(SX, SY)) return;
	
	const float X = (SX * (InSectionWidth));
	const float Z = -(SY * (InSectionHeight));
	const FVector SectionLocation = FVector(X, 0, Z) + InWorldOffset;

	if(ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity))
	{
		NewSection->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NewSection->SetFolderPath(GetFolderPath());
		NewSection->InitializeSection(FVector2D(InSectionWidth, InSectionHeight),InSection, InProceduralAsset);
		NewSection->FinishSpawning(FTransform::Identity);
		NewSection->SetActorLocation(SectionLocation);
		SectionActors.Add(NewSection);
	}
}

void ADigumWorldProceduralActor::CreateSection(FDigumWorldProceduralSection InSection)
{
	CreateSection(UnitSectionSize.X, UnitSectionSize.Y, WorldOffset, InSection, ProceduralAsset);
}

void ADigumWorldProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - WorldOffset;
	
	// Translate World Position to Section Coordinates
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, UnitSectionSize, SectionCoordinate);

	UE_LOG(LogTemp, Warning, TEXT("Section Coordinate %s"), *SectionCoordinate.ToString());
	// Loop through all sections and add block
	// TODO: Optimize this, currently looping through all sections...
    SectionActors.FindByPredicate([&](ADigumWorldActorSection*& SectionActor)
    {
		if(SectionActor == nullptr) return false;
    	FDigumWorldProceduralSection SectionData = SectionActor->GetSectionData();
        if(SectionData.GetX() == FMath::Abs(SectionCoordinate.X) && SectionData.GetY() ==  FMath::Abs(SectionCoordinate.Y))
        {
        	SectionActor->AddBlock(InBlockID, LocalPosition, LocalSectionWidth, LocalSectionHeight);
            return true;
        }
        
        return false;
    });
}

void ADigumWorldProceduralActor::ApplyWorldOffsetPosition()
{
	SetActorLocation(WorldOffset);
}

ADigumWorldActorSection* ADigumWorldProceduralActor::GetSectionActor(const int32& InX, const int32& InY) const
{
	for(auto& Child : SectionActors)
	{
		if(Child)
		{
			if(Child->GetSectionData().GetX() == InX && Child->GetSectionData().GetY() == InY)
			{
				return Child;
			}
		}
	}

	return nullptr;
}

FDigumWorldProceduralSectionCoordinate ADigumWorldProceduralActor::GetSectionCoordinate(
	const FVector& InWorldLocation) const
{
	const FVector OffsettedLocation = InWorldLocation - WorldOffset;
	const int32 X = FMath::FloorToInt(OffsettedLocation.X / UnitSectionSize.X);
	const int32 Y = -FMath::CeilToInt((OffsettedLocation.Z) / UnitSectionSize.Y);
	const int32 AbsX = FMath::Abs(X);
	const int32 AbsY = FMath::Abs(Y);
	return FDigumWorldProceduralSectionCoordinate(AbsX, AbsY);
}

void ADigumWorldProceduralActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumWorldProceduralActor, Map);
	DOREPLIFETIME(ADigumWorldProceduralActor, SectionDataArray);
	DOREPLIFETIME(ADigumWorldProceduralActor, SectionActors);
	DOREPLIFETIME(ADigumWorldProceduralActor, UnitSectionSize);
	DOREPLIFETIME(ADigumWorldProceduralActor, LocalSectionWidth);
	DOREPLIFETIME(ADigumWorldProceduralActor, LocalSectionHeight);
	DOREPLIFETIME(ADigumWorldProceduralActor, GridSize);
	DOREPLIFETIME(ADigumWorldProceduralActor, WorldOffset);
	/*	UPROPERTY(Replicated)
	FVector2D UnitSectionSize;
	
	UPROPERTY(Replicated)
	int32 LocalSectionWidth;

	UPROPERTY(Replicated)
	int32 LocalSectionHeight;

	UPROPERTY(Replicated)
	FVector GridSize;

	UPROPERTY(Replicated)
	FVector WorldOffset;
	*/
}

void ADigumWorldProceduralActor::Editor_GenerateProceduralWorld()
{
	FName ContentCategoryName = TEXT("Primary");
	if(const FDigumWorldContentCategory* ContentCategory = UDigumWorldSettings::Get()->GetWorldContentCategoryData(ContentCategoryName))
	{
		const FDigumWorldProceduralRules* Rules = &ContentCategory->ProceduralRules;
		if(Rules == nullptr) return;

		if(UDigumWorldProceduralAsset* Asset = Rules->GetProceduralAsset())
		{
			FDigumWorldProceduralMap ProceduralMap;
			UDigumWorldGenerator::GenerateWorldMap(*Rules, ProceduralMap);	
			
			if(ProceduralMap.GetSectionCount() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("No sections generated"));
				return; 
			}
			UDigumWorldFunctionHelpers::GetLocalSectionSize(ContentCategoryName, LocalSectionWidth, LocalSectionHeight);
			const FVector EditorGridSize = UDigumWorldSettings::GetGridSize();
			const float SectionWidth = Rules->SectionWidth * EditorGridSize.X;
			const float SectionHeight = Rules->SectionHeight * EditorGridSize.Z;

			SetActorLocation(FVector(0, 0, 0));

			UnitSectionSize = FVector2D(SectionWidth, SectionHeight);
			
			
			for(auto& Section : ProceduralMap.GetSections())
			{
				CreateSection(SectionWidth, SectionHeight, FVector::ZeroVector, Section, ProceduralAsset);
			}

			const float TotalWidth = Rules->SectionCount_HorizontalAxis * SectionWidth;
			const float TotalHeight = Rules->SectionCount_VerticalAxis * SectionHeight;

			SetActorLocation(FVector(-TotalWidth / 2, 0, TotalHeight / 2));
		}
		
#if WITH_EDITOR
		MarkPackageDirty();
#endif
	}
}


void ADigumWorldProceduralActor::Editor_CleanActors()
{
	for(auto& Child : SectionActors)
	{
		if(Child)
		{
			Child->DestroySection();
		}
	}
#if WITH_EDITOR
	MarkPackageDirty();
#endif
}

