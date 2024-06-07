// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Actor/DigumWorldActorChild.h"
#include "Actor/DigumWorldActorSection.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Functions/DigumWorldFunctionHelpers.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldProceduralActor::ADigumWorldProceduralActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

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

void ADigumWorldProceduralActor::GenerateMap(const FName InSeed,  const FVector InGridSize, const int32 InSectionWidth,
	const int32 InSectionHeight, int32 InSectionCount_HorizontalAxis, const int32 InSectionCount_VerticalAxis,
	const int32 InNumberOfHierarchies)
{
	Map = FDigumWorldMap(InSeed, InGridSize, InSectionWidth, InSectionHeight, InSectionCount_HorizontalAxis, InSectionCount_VerticalAxis, InNumberOfHierarchies);
	LocalSectionWidth = InSectionWidth;
	LocalSectionHeight = InSectionHeight;
	GridSize = InGridSize;
	UnitSectionSize = Map.GetSectionUnitSize();
	WorldOffset = Map.GetWorldOffset();
}

bool ADigumWorldProceduralActor::GetSection(const int32& InSectionX, const int32& InSectionY,
	FDigumWorldProceduralSection& OutSection) const
{
	if(ProceduralAsset == nullptr) return false;

	return UDigumWorldGenerator::GenerateSection(Map, InSectionX, InSectionY, ProceduralAsset, OutSection);
}


void ADigumWorldProceduralActor::CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, 
                                               FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* InProceduralAsset)
{
	const int32 SX = InSection.GetX();
	const int32 SY = InSection.GetY();

	if(SX < 0 || SY < 0) return;
	if(GetSectionActor(SX, SY) != nullptr) return;
	
	const float X = (SX * (InSectionWidth));
	const float Z = -(SY * (InSectionHeight));
	const FVector SectionLocation = FVector(X, 0, Z) + InWorldOffset;

	ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity);
	if(NewSection)
	{
		NewSection->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NewSection->SetFolderPath(GetFolderPath());
		NewSection->InitializeSection(FVector2D(InSectionWidth, InSectionHeight),InSection, InProceduralAsset);
		NewSection->FinishSpawning(FTransform::Identity);
		NewSection->SetActorLocation(SectionLocation);
		SectionActors.Add(NewSection);
	}
}

void ADigumWorldProceduralActor::CreateSection(FDigumWorldProceduralSection& InSection)
{
	CreateSection(UnitSectionSize.X, UnitSectionSize.Y, WorldOffset, InSection, ProceduralAsset);
}

void ADigumWorldProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - GetActorLocation();
	
	// Translate World Position to Section Coordinates
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, UnitSectionSize, SectionCoordinate);

	UE_LOG(LogTemp, Warning, TEXT("Section Coordinate %s"), *SectionCoordinate.ToString());
	// Loop through all sections and add block
	// TODO: Optimize this, currently looping through all sections...
    SectionActors.FindByPredicate([&](ADigumWorldActorSection*& Section)
    {
		if(Section == nullptr) return false;
    	FDigumWorldProceduralSection SectionData = Section->GetSectionData();
        if(SectionData.GetX() == FMath::Abs(SectionCoordinate.X) && SectionData.GetY() ==  FMath::Abs(SectionCoordinate.Y))
        {
        	Section->AddBlock(InBlockID, LocalPosition, LocalSectionWidth, LocalSectionHeight);
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
	const FVector OffsetedLocation = InWorldLocation - WorldOffset;
	const int32 X = FMath::FloorToInt(OffsetedLocation.X / UnitSectionSize.X);
	const int32 Y = -FMath::CeilToInt((OffsetedLocation.Z) / UnitSectionSize.Y);
	const int32 AbsX = FMath::Abs(X);
	const int32 AbsY = FMath::Abs(Y);
	return FDigumWorldProceduralSectionCoordinate(AbsX, AbsY);
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

