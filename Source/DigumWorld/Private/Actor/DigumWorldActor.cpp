// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActor.h"
#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Asset/DigumWorldAsset.h"


ADigumWorldActor::ADigumWorldActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADigumWorldActor::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	SetFolderPath(TEXT("/DigumWorld"));
#endif
	
}

void ADigumWorldActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR	
void ADigumWorldActor::Editor_UpdateWorldAsset()
{
	if(WorldAsset == nullptr) return;

	Editor_CleanActors();
	
	for(int32 i = 0; i < WorldAsset->GetLayers().Num(); i++)
	{
		TArray<FDigumWorldAssetCoordinate> Coordinates = WorldAsset->GetCoordinates(i);
		TMap<FName, FDigumWorldAssetCoordinateArray> Group;
		for(int32 c = 0; c < Coordinates.Num(); c++)
		{
			FName SwatchName = Coordinates[c].SwatchName;
			FDigumWorldAssetCoordinate Coordinate = Coordinates[c];
			UE_LOG(LogTemp, Warning, TEXT("SwatchName: %s"), *SwatchName.ToString());
			if(Group.Contains(SwatchName))
			{
				FDigumWorldAssetCoordinateArray& GroupCoordinates = Group[SwatchName];
				GroupCoordinates.AddCoordinate(Coordinate);
			}
			else
			{
				FDigumWorldAssetCoordinateArray GroupCoordinates;
				GroupCoordinates.AddCoordinate(Coordinate);
				Group.Add(SwatchName, GroupCoordinates);
			}
		}
		for (auto It = Group.CreateConstIterator(); It; ++It)
		{
			FName Key = It.Key();
			FDigumWorldAssetCoordinateArray Value = It.Value();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			
			UDigumWorldSwatchAsset* Swatch = UDigumAssetManager::GetAsset<UDigumWorldSwatchAsset>(WorldAsset->GetSwatch(Key)->SoftSwatchAsset);

			if(Swatch)
			{
				if(TSubclassOf<ADigumWorldActorChild> ChildClass = Swatch->GetChildActorClass())
				{
					if(ADigumWorldActorChild* NewActor = GetWorld()->SpawnActorDeferred<ADigumWorldActorChild>(ChildClass, FTransform::Identity))
					{
						NewActor->InitializeSwatchAsset(Swatch, Value);
						NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
						NewActor->SetFolderPath(GetFolderPath());
						NewActor->FinishSpawning(FTransform::Identity);
						WorldChildActors.Add(NewActor);
					}
				}
			}
			
		}
	
	}
	
	MarkPackageDirty();
}

void ADigumWorldActor::Editor_CleanActors()
{
	for(TWeakObjectPtr<AActor> Child : WorldChildActors)
	{
		if(Child.IsValid())
		{
			Child->Destroy();
		}
	}
	MarkPackageDirty();
}
#endif
