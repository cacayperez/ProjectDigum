// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorChild.h"

#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Components/DigumWorldISMComponent.h"
#include "Settings/DigumWorldSettings.h"

ADigumWorldActorChild::ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	InstancedMeshComponent = CreateDefaultSubobject<UDigumWorldISMComponent>(TEXT("InstancedMeshComponent"));
}

void ADigumWorldActorChild::OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset,
	FDigumWorldAssetCoordinateArray Coordinates)
{
}

void ADigumWorldActorChild::InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset,
                                               FDigumWorldAssetCoordinateArray Coordinates)
{
	SwatchAsset = InSwatchAsset;
	if(SwatchAsset)
	{
		InstancedMeshComponent->ClearInstances();
		int32 GridSize = GetDefault<UDigumWorldSettings>()->GridSize;
		UStaticMesh* Mesh = UDigumAssetManager::GetAsset<UStaticMesh>(SwatchAsset->SwatchMesh);
		if(Mesh)
		{
			InstancedMeshComponent->SetStaticMesh(Mesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh is nullll"));
		}
		const float HalfGrid = GridSize * 0.5f;
		
		for(int32 i = 0; i < Coordinates.CoordinateCount(); i++)
		{
			FDigumWorldAssetCoordinate* Coordinate = Coordinates.GetAt(i);
			// Since this is a 2D grid, we can use the X and Y coordinates to determine the location of the instance
			const float X = Coordinate->X * HalfGrid;
			const float Y = 0.0f;
			const float Z = -Coordinate->Y * HalfGrid;
			FVector Location = FVector(X, Y, Z);
			FTransform Transform = FTransform(FRotator::ZeroRotator, Location, FVector(1.0f));
			InstancedMeshComponent->AddInstance(Transform);
		}

		OnFinishedInitializeSwatchAsset(SwatchAsset, Coordinates);
	}
}


