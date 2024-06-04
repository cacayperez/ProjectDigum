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

void ADigumWorldProceduralActor::CreateSection(const float& InSectionWidth, const float& InSectionHeight,
	FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	const float SX = InSection.GetX();
	const float SY = InSection.GetY();
	const float X = (SX * (InSectionWidth));
	const float Z = -(SY * (InSectionHeight));
	FVector SectionLocation = FVector(X, 0, Z);

	ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity);
	if(NewSection)
	{
		NewSection->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewSection->SetFolderPath(GetFolderPath());
		NewSection->InitializeSection(FVector2D(InSectionWidth, InSectionHeight),InSection, ProceduralAsset);
		NewSection->FinishSpawning(FTransform::Identity);
		NewSection->SetActorLocation(SectionLocation);
		// UE_LOG(LogTemp, Warning, TEXT("Section spawned %s"), *SectionLocation.ToString());
		SectionActors.Add(NewSection);

#if WITH_EDITOR
		NewSection->bHiddenEd = true;
#endif
	}
}

void ADigumWorldProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	// Get Local Position
	const FVector LocalPosition = InBlockLocation - GetActorLocation();
	
	// Translate World Position to Section Coordinates
	FDigumWorldProceduralSectionCoordinate SectionCoordinate;
	UDigumWorldFunctionHelpers::ConvertToSectionCoordinates(LocalPosition, SectionSize, SectionCoordinate);

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
			const FVector GridSize = UDigumWorldSettings::GetGridSize();
			const float SectionWidth = Rules->SectionWidth * GridSize.X;
			const float SectionHeight = Rules->SectionHeight * GridSize.Z;

			SetActorLocation(FVector(0, 0, 0));

			SectionSize = FVector2D(SectionWidth, SectionHeight);
			
			for(auto& Section : ProceduralMap.GetSections())
			{
				CreateSection(SectionWidth, SectionHeight, Section, ProceduralAsset);
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
	for(TWeakObjectPtr<ADigumWorldActorSection> Child : SectionActors)
	{
		if(Child.IsValid())
		{
			Child->DestroySection();
		}
	}
#if WITH_EDITOR
	MarkPackageDirty();
#endif
}

