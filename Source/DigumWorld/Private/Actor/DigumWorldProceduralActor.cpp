// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldProceduralActor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Actor/DigumWorldActorChild.h"
#include "Actor/DigumWorldActorSection.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Settings/DigumWorldSettings.h"


// Sets default values
ADigumWorldProceduralActor::ADigumWorldProceduralActor()
{
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

void ADigumWorldProceduralActor::CreateSection(const float& InSectionWidth, const float& InSectionHeight,
	FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset)
{
	const FDigumWorldProceduralCoordinateArray* CoordinateArray = InSection.GetCoordinateArray();
	if(CoordinateArray == nullptr) return;;
	
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
		NewSection->InitializeSection(InSection, ProceduralAsset);
		NewSection->FinishSpawning(FTransform::Identity);
		NewSection->SetActorLocation(SectionLocation);
		// UE_LOG(LogTemp, Warning, TEXT("Section spawned %s"), *SectionLocation.ToString());
		SectionActors.Add(NewSection);
	}
}

void ADigumWorldProceduralActor::AddBlock(const FName& InBlockID, const FVector& InBlockLocation)
{
	const int32 SectionX = FMath::FloorToInt(InBlockLocation.X / SectionSize.X);
	const int32 SectionY = FMath::FloorToInt(InBlockLocation.Z / SectionSize.Y);

	for(ADigumWorldActorSection* Section : SectionActors)
	{
		if(Section == nullptr) continue;
		
		UE_LOG(LogTemp, Warning, TEXT("Place Section %d, %d"), SectionX, SectionY);
		if(Section->GetX() == SectionX && Section->GetY() == SectionY)
		{
			UE_LOG(LogTemp, Warning, TEXT("Section %d, %d"), Section->GetX(), Section->GetY());
			Section->AddBlock(InBlockID, InBlockLocation);
			return;
		}
	}
	
    /*SectionActors.FindByPredicate([SectionX, SectionY, InBlockID, InBlockLocation](const TWeakObjectPtr<ADigumWorldActorSection>& Section)
    {
        if(const FDigumWorldProceduralSection* SectionData = Section->GetSectionData())
        {
            if(SectionData->GetX() == SectionX && SectionData->GetY() == SectionY)
            {
                Section->AddBlock(InBlockID, InBlockLocation);
                return true;
            }
        }
        return false;
    });*/

	// TODO: Handle if section not found
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

			const FVector GridSize = UDigumWorldSettings::GetGridSize();
			const float SectionWidth = Rules->SectionWidth * GridSize.X;
			const float SectionHeight = Rules->SectionHeight * GridSize.Z;

			SectionSize = FVector2D(SectionWidth, SectionHeight);
			
			for(auto& Section : ProceduralMap.GetSections())
			{
				CreateSection(SectionWidth, SectionHeight, Section, ProceduralAsset);
			}
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

