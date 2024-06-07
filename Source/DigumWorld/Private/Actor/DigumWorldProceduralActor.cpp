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

void ADigumWorldProceduralActor::GenerateMap(const FName& InContentCategoryName)
{
	FName ContentCategoryName = InContentCategoryName;
	if(const FDigumWorldContentCategory* ContentCategory = UDigumWorldSettings::Get()->GetWorldContentCategoryData(ContentCategoryName))
	{
		const FDigumWorldProceduralRules* Rules = &ContentCategory->ProceduralRules;
		if(Rules == nullptr) return;

		FDigumWorldProceduralMap ProceduralMap;
		UDigumWorldGenerator::GenerateWorldMap(*Rules, ProceduralMap);
	}
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

void ADigumWorldProceduralActor::Initialize(const int32& InLocalSectionWidth, const int32& InLocalSectionHeight,
	const FVector& InGridSize)
{
	LocalSectionWidth = InLocalSectionWidth;
	LocalSectionHeight = InLocalSectionHeight;
	GridSize = InGridSize;
	SectionSize = FVector2D(InLocalSectionWidth * InGridSize.X, InLocalSectionHeight * InGridSize.Z);
}


void ADigumWorldProceduralActor::CreateSection(const float& InSectionWidth, const float& InSectionHeight, const FVector& InWorldOffset, 
                                               FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	
	const float SX = InSection.GetX();
	const float SY = InSection.GetY();

	if(GetSectionActor(SX, SY) != nullptr) return;
	
	const float X = (SX * (InSectionWidth));
	const float Z = -(SY * (InSectionHeight));
	const FVector SectionLocation = FVector(X, 0, Z) + InWorldOffset;

	ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity);
	if(NewSection)
	{
		NewSection->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NewSection->SetFolderPath(GetFolderPath());
		NewSection->InitializeSection(FVector2D(InSectionWidth, InSectionHeight),InSection, ProceduralAsset);
		NewSection->FinishSpawning(FTransform::Identity);
		NewSection->SetActorLocation(SectionLocation);
		// UE_LOG(LogTemp, Warning, TEXT("Section spawned %s"), *SectionLocation.ToString());
		SectionActors.Add(NewSection);
	}
}

void ADigumWorldProceduralActor::CreateSection(FDigumWorldProceduralSection& InSection, const FVector& InWorldOffset, 
	UDigumWorldProceduralAsset* ProceduralAsset)
{
	CreateSection(SectionSize.X, SectionSize.Y, InWorldOffset, InSection, ProceduralAsset);
}

void ADigumWorldProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - GetActorLocation();
	
	// Translate World Position to Section Coordinates
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, SectionSize, SectionCoordinate);

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

void ADigumWorldProceduralActor::Editor_GenerateProceduralWorld()
{
	FName ContentCategoryName = TEXT("Primary");
	if(const FDigumWorldContentCategory* ContentCategory = UDigumWorldSettings::Get()->GetWorldContentCategoryData(ContentCategoryName))
	{
		const FDigumWorldProceduralRules* Rules = &ContentCategory->ProceduralRules;
		if(Rules == nullptr) return;

		if(UDigumWorldProceduralAsset* ProceduralAsset = Rules->GetProceduralAsset())
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

			SectionSize = FVector2D(SectionWidth, SectionHeight);
			
			
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

