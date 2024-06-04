// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorChild.h"

#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Components/DigumWorldISMComponent.h"
#include "Settings/DigumWorldSettings.h"
#include "Subsystem/DigumWorldSubsystem.h"

ADigumWorldActorChild::ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	PrimaryActorTick.bCanEverTick = false;
	InstancedMeshComponent = CreateDefaultSubobject<UDigumWorldISMComponent>(TEXT("InstancedMeshComponent"));
	InstancedMeshComponent->SetupAttachment(Root);

}

void ADigumWorldActorChild::BeginPlay()
{
	Super::BeginPlay();

	
	// Initialize Health
	/*int32 InstanceCount = InstancedMeshComponent->GetInstanceCount();
	for(int32 i = 0; i < InstanceCount; i++)
	{
		Health.Add(1.0f);
	}*/
	
}

void ADigumWorldActorChild::OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset,
                                                            FDigumWorldAssetCoordinateArray Coordinates)
{
}

void ADigumWorldActorChild::BuildChildProperties(UDigumWorldSwatchAsset* InSwatchAsset)
{
}

bool ADigumWorldActorChild::GetInstancedHitIndex(const FVector HitLocation, const float& InMaxRange, int32& OutIndex)
{
	
	// Iterate over all instances to find the one closest to the hit location
	int32 ClosestInstanceIndex = -1;
	float MinDistanceSquared = InMaxRange;
	const int32 InstanceCount = InstancedMeshComponent->GetInstanceCount();
	const FVector ActorLocation = GetActorLocation();
	for(int32 i = 0; i < InstanceCount; ++i)
	{
		FTransform InstanceTransform;
		if(InstancedMeshComponent->GetInstanceTransform(i, InstanceTransform))
		{
			
			float DistanceSquared = (ActorLocation + InstanceTransform.GetLocation() - HitLocation).SizeSquared();
			if(DistanceSquared < MinDistanceSquared)
			{
				MinDistanceSquared = DistanceSquared;
				ClosestInstanceIndex = i;
			}
		}
	}
	
	OutIndex =  ClosestInstanceIndex;

	if(ClosestInstanceIndex != -1)
	{
		return true;
	}
	
	return false;
}

void ADigumWorldActorChild::OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation)
{
	
}

void ADigumWorldActorChild::InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset,
                                                  FDigumWorldAssetCoordinateArray Coordinates, const int32 HierarchyIndex)
{
	SwatchAsset = InSwatchAsset;
	
	if(SwatchAsset)
	{
		BuildChildProperties(SwatchAsset);
		
		Health.Empty();
		InstancedMeshComponent->ClearInstances();
		GridSize = GetDefault<UDigumWorldSettings>()->GridSize;
		UStaticMesh* Mesh = UDigumAssetManager::GetAsset<UStaticMesh>(SwatchAsset->SwatchMesh);
		if(Mesh)
		{
			InstancedMeshComponent->SetStaticMesh(Mesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh is nullll"));
		}
		const float GridX = GridSize.X;
		const float GridY = GridSize.Y;
		const float GridZ = GridSize.Z;
		
		for(int32 i = 0; i < Coordinates.CoordinateCount(); i++)
		{
			FDigumWorldAssetCoordinate* Coordinate = Coordinates.GetAt(i);
			// Since this is a 2D grid, we can use the X and Y coordinates to determine the location of the instance
			const float X = Coordinate->X * GridX;
			const float Y = HierarchyIndex * GridY;
			const float Z = -((Coordinate->Y * GridZ) + GridZ);
			FVector Location = FVector(X, Y, Z);
			FTransform Transform = FTransform(FRotator::ZeroRotator, Location, FVector(1.0f));
			
			int32 InstanceIndex = InstancedMeshComponent->AddInstance(Transform);
			InstancedMeshComponent->SetTint(InstanceIndex, HierarchyIndex);
		}

		OnFinishedInitializeSwatchAsset(SwatchAsset, Coordinates);
	}
}

void ADigumWorldActorChild::InitializeSwatchAsset(const FName& InBlockID, UDigumWorldSwatchAsset* InSwatchAsset,
	FDigumWorldProceduralCoordinateArray Coordinates)
{
	SwatchAsset = InSwatchAsset;
	BlockID = InBlockID;
	
	if(SwatchAsset)
	{
		BuildChildProperties(SwatchAsset);
		Health.Empty();
		InstancedMeshComponent->ClearInstances();
		GridSize = GetDefault<UDigumWorldSettings>()->GridSize;
		if(UStaticMesh* Mesh = UDigumAssetManager::GetAsset<UStaticMesh>(SwatchAsset->SwatchMesh))
		{
			InstancedMeshComponent->SetStaticMesh(Mesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh is null"));
		}

		AddBlock(Coordinates);
	}
	
}

void ADigumWorldActorChild::SetWorldCollision(const bool& bValue)
{
	if(bValue == true)
	{
		InstancedMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		InstancedMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
	}
}

void ADigumWorldActorChild::AddBlock(FDigumWorldProceduralCoordinateArray& InCoordinates)
{
	const float GridX = GridSize.X;
	const float GridY = GridSize.Y;
	const float GridZ = GridSize.Z;
		
	for(int32 i = 0; i < InCoordinates.CoordinateCount(); i++)
	{
		FDigumWorldProceduralCoordinate* Coordinate = InCoordinates.GetCoordinate(i);
		// Since this is a 2D grid, we can use the X and Y coordinates to determine the location of the instance
		const float X = Coordinate->X * GridX;
		const float Y = Coordinate->Hierarchy * GridY;
		const float Z = -(Coordinate->Y * GridZ);
		FVector Location = FVector(X, Y, Z);
		FTransform Transform = FTransform(FRotator::ZeroRotator, Location, FVector(1.0f));
			
		int32 InstanceIndex = InstancedMeshComponent->AddInstance(Transform);
		InstancedMeshComponent->SetTint(InstanceIndex, Coordinate->Hierarchy);
		Health.Add(1.0f);
	}
}

void ADigumWorldActorChild::OnCollide(AActor* InInstigator, const FVector& InLocation, const int32& InIndex)
{
	/*if(InIndex == INDEX_NONE) return; 

	FTransform Transform;
	int32 OutIndex = -1;
	if(GetInstancedHitIndex(InLocation, OutIndex))
	{
		float Distance = FVector::Distance(InLocation, Transform.GetLocation());
		if(OutIndex >= 0)
		{
			InstancedMeshComponent->RemoveInstance(InIndex);
		}
	}*/
}

void ADigumWorldActorChild::DestroyInstance(const FVector& InLocation, const float& InMaxRange)
{
	float Distance = FVector::Distance(InLocation, GetActorLocation());
	if(Distance > InMaxRange) return;
	int32 OutIndex;
	if(GetInstancedHitIndex(InLocation, InMaxRange, OutIndex))
	{
		if(OutIndex >= 0)
		{
			InstancedMeshComponent->RemoveInstance(OutIndex);
			Health.RemoveAt(OutIndex);
		}
	}
}

void ADigumWorldActorChild::DestroyInstance(const int32& InIndex)
{
	if(InIndex == INDEX_NONE) return;

	FTransform Transform;
	if(InstancedMeshComponent->GetInstanceTransform(InIndex, Transform, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyInstance %s"), *Transform.GetLocation().ToString());
		InstancedMeshComponent->RemoveInstance(InIndex);
		OnDestroyChildInstance(InIndex, Transform.GetLocation());
		
	}
}

void ADigumWorldActorChild::OnInteract_Implementation(const AActor* InInstigator,
	const FDigumWorldRequestParams& InParams)
{
	if(InParams.Request == EDigumWorld_Request::DigumWorldRequest_Destroy)
	{
		// UE_LOG(LogTemp, Warning, TEXT("HitInstanceIndex: %i, %f"), InParams.HitInstanceIndex, InParams.Magnitude);
		if(Health.IsValidIndex(InParams.HitInstanceIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("HitInstanceIndex: %i, %f"), InParams.HitInstanceIndex, InParams.Magnitude);
			Health[InParams.HitInstanceIndex] -= InParams.Magnitude;
			if(Health[InParams.HitInstanceIndex] <= 0.0f)
			{
				DestroyInstance(InParams.HitInstanceIndex);
			}
			UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health[InParams.HitInstanceIndex]);
			
		}
	}
}




