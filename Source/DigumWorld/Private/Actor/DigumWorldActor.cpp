// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActor.h"

#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumWorldAsset.h"

// Sets default values
ADigumWorldActor::ADigumWorldActor()
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
		TMap<FName, TArray<FDigumWorldAssetCoordinate>> Group;
		for(int32 c = 0; c < Coordinates.Num(); c++)
		{
			FName SwatchName = Coordinates[c].SwatchName;
			FDigumWorldAssetCoordinate Coordinate = Coordinates[c];
			UE_LOG(LogTemp, Warning, TEXT("SwatchName: %s"), *SwatchName.ToString());
			if(Group.Contains(SwatchName))
			{
				TArray<FDigumWorldAssetCoordinate>& GroupCoordinates = Group[SwatchName];
				GroupCoordinates.Add(Coordinate);
			}
			else
			{
				TArray<FDigumWorldAssetCoordinate> GroupCoordinates;
				GroupCoordinates.Add(Coordinate);
				Group.Add(SwatchName, GroupCoordinates);
			}
		}
		for (auto It = Group.CreateConstIterator(); It; ++It)
		{
			FName Key = It.Key();
			TArray<FDigumWorldAssetCoordinate> Value = It.Value();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			
			AActor* NewActor = GetWorld()->SpawnActor<AActor>(ADigumWorldActorChild::StaticClass(), FTransform::Identity, SpawnParams);
			if(NewActor)
			{
				NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				NewActor->SetFolderPath(GetFolderPath());
				WorldChildActors.Add(NewActor);
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
