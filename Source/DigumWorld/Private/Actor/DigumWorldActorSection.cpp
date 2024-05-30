// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorSection.h"

#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Procedural/DigumWorldGenerator.h"


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

void ADigumWorldActorSection::InitializeSection(FDigumWorldProceduralMappedCoordinates* MappedCoordinates, UDigumWorldProceduralAsset* ProceduralAsset)
{
	TMap<FName, FDigumWorldProceduralCoordinateArray>* Blocks = MappedCoordinates->GetMappedCoordinates();

	for (auto It = Blocks->CreateConstIterator(); It; ++It)
	{
		FName BlockID = It->Key;
		FDigumWorldProceduralCoordinateArray BlockCoordinates = It->Value;

		UDigumWorldSwatchAsset* SwatchAsset = ProceduralAsset->GetSwatchAsset(BlockID);

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
					NewActor->InitializeSwatchAsset(SwatchAsset, BlockCoordinates, 0);
					NewActor->FinishSpawning(ChildTransform);
					
					
					WorldChildActors.Add(NewActor);
				}
			}
		}
	}
}

void ADigumWorldActorSection::DestroySection()
{
	for(TWeakObjectPtr<AActor> Child : WorldChildActors)
	{
		if(Child.IsValid())
		{
			Child->Destroy();
		}
	}

	Destroy();
}

