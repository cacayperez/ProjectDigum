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

			FVector GridSize = UDigumWorldSettings::GetGridSize();
			const float HalfGridX = GridSize.X * 0.5f;
			const float HalfGridZ = GridSize.Z * 0.5f;
			float SectionWidth = Rules->SectionWidth * GridSize.X;
			float SectionHeight = Rules->SectionHeight * GridSize.Z;

			for(auto& Section : ProceduralMap.GetSections())
			{
				// TODO implement Hierarchies
				FDigumWorldProceduralCoordinateArray* CoordinateArray = Section.GetCoordinateArray();
				if(CoordinateArray == nullptr) continue;
				const float SX = Section.GetX();
				const float SY = Section.GetY();
				const float X = (SX * (SectionWidth));
				const float Z = -(SY * (SectionHeight));
				FVector SectionLocation = FVector(X, 0, Z);
				FTransform SectionTransform = FTransform(FRotator::ZeroRotator, SectionLocation);

				ADigumWorldActorSection* NewSection = GetWorld()->SpawnActorDeferred<ADigumWorldActorSection>(ADigumWorldActorSection::StaticClass(), FTransform::Identity);
				if(NewSection)
				{
					NewSection->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
					NewSection->SetFolderPath(GetFolderPath());
					NewSection->InitializeSection(Section, ProceduralAsset);
					NewSection->FinishSpawning(FTransform::Identity);
					NewSection->SetActorLocation(SectionLocation);
					// UE_LOG(LogTemp, Warning, TEXT("Section spawned %s"), *SectionLocation.ToString());
					SectionActors.Add(NewSection);
				}
			}
		}
		

		MarkPackageDirty();
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
	MarkPackageDirty();
}

