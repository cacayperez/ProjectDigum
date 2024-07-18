// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorChild.h"

#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldAsset.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "Async/DigumWorldAsyncBlock.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Components/DigumWorldISMComponent.h"
#include "Net/UnrealNetwork.h"
#include "Settings/DigumWorldSettings.h"
#include "Subsystem/DigumWorldSubsystem.h"

ADigumWorldActorChild::ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	PrimaryActorTick.bCanEverTick = true;
	InstancedMeshComponent = CreateDefaultSubobject<UDigumWorldISMComponent>(TEXT("InstancedMeshComponent"));
	InstancedMeshComponent->SetupAttachment(Root);

	bReplicates = true;
}

void ADigumWorldActorChild::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(ADigumWorldActorChild, SwatchAsset);
	DOREPLIFETIME(ADigumWorldActorChild, BlockID);
	DOREPLIFETIME(ADigumWorldActorChild, Health);
	DOREPLIFETIME(ADigumWorldActorChild, SectionWidth);
	DOREPLIFETIME(ADigumWorldActorChild, SectionHeight);
	
	DOREPLIFETIME(ADigumWorldActorChild, GridSize);
	DOREPLIFETIME(ADigumWorldActorChild, SwatchAsset);
}

void ADigumWorldActorChild::BeginPlay()
{
	Super::BeginPlay();
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
			
			float DistanceSquared = (InstanceTransform.GetLocation() - HitLocation).SizeSquared();
			
			if(DistanceSquared < MinDistanceSquared)
			{
				MinDistanceSquared = DistanceSquared;
				ClosestInstanceIndex = i;
			}
		}
	}
	
	OutIndex =  ClosestInstanceIndex;

	if(ClosestInstanceIndex <= 0)
	{
		return false;
	}
	
	return true;
}

void ADigumWorldActorChild::OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation)
{
	
}

void ADigumWorldActorChild::AsyncAddBlock()
{
	TSharedPtr<FDigumWorldAsyncBlockResultArray> ArrayResultPtr;

	if(!AsyncBlockResultArrayQueue.IsEmpty())
	{
		AsyncBlockResultArrayQueue.Peek(ArrayResultPtr);
		if(ArrayResultPtr)
		{
			// TArray<FTransform> GeneratedTransform;
			for(auto& Result : ArrayResultPtr->ResultArray)
			{
				if(InstancedMeshComponent)
				{
					const FTransform Transform = Result.Transform;
					const int32 HierarchyIndex = Result.Coordinate.Hierarchy;
					const int32 Variant = Result.Variant;
					const bool bHasTopNeighbor = Result.Coordinate.bHasTopNeighbor;
					const int32 X = Result.Coordinate.X;
					const int32 Y = Result.Coordinate.Y;
					const int32 LocalIndex = (SectionWidth * Y) + X;
					// UE_LOG(LogTemp, Warning, TEXT("LocalIndex, %i"), LocalIndex);
					InstancedMeshComponent->AddWorldInstance(Transform, HierarchyIndex, Variant, LocalIndex, bHasTopNeighbor);
				}
			}
			SetActorTickEnabled(false);
			AsyncBlockResultArrayQueue.Dequeue(ArrayResultPtr);
		}
		
		ArrayResultPtr.Reset();
	}

}

void ADigumWorldActorChild::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AsyncAddBlock();
}

void ADigumWorldActorChild::InitializeISMComponent(const int32& InHierarchyCount, const int32& InSectionWidth,
	const int32& InSectionHeight)
{
	if(InstancedMeshComponent)
	{
		InstancedMeshComponent->InitializeSize(InHierarchyCount, InSectionWidth, InSectionHeight);
	}
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
			const float Y = (HierarchyIndex * 3) * GridY;
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
	FDigumWorldProceduralCoordinateArray Coordinates, const int32& NumOfHierarchies, const int32& InSectionWidth, const int32& InSectionHeight)
{
	SwatchAsset = InSwatchAsset;
	BlockID = InBlockID;
	SectionWidth = InSectionWidth;
	SectionHeight = InSectionHeight;

	InitializeISMComponent(NumOfHierarchies, SectionWidth, SectionHeight);
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

		
		AddBlock(InBlockID,Coordinates);
		
	}
	
}

void ADigumWorldActorChild::ResetChildActor()
{
	// SetActorEnableCollision(false);
	// SetActorHiddenInGame(true);
	Health.Empty();
	InstancedMeshComponent->ClearInstances();
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

void ADigumWorldActorChild::AddBlock(const FName& InBlockID, FDigumWorldProceduralCoordinateArray& InCoordinates)
{
	UE_LOG(LogTemp, Warning, TEXT("Coordinate Count %i,"), InCoordinates.CoordinateCount());
	
	const float GridX = GridSize.X;
	const float GridY = GridSize.Y;
	const float GridZ = GridSize.Z;

	const FVector PositionOffset = SwatchAsset->GetPositionOffset();
	SetActorTickEnabled(true);

	

	AsyncTask(ENamedThreads::AnyThread, [this, InBlockID, PositionOffset, InCoordinates]
	{
		(new FAutoDeleteAsyncTask<FDigumWorldAsyncBlock>(this, InBlockID, GridSize, PositionOffset, InCoordinates))->StartBackgroundTask();
	});
	
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
	/*float Distance = FVector::Distance(InLocation, GetActorLocation());
	if(Distance > InMaxRange) return;*/
	int32 OutIndex;
	UE_LOG(LogTemp, Warning, TEXT("Hitttt"));
	if(GetInstancedHitIndex(InLocation, InMaxRange, OutIndex))
	{
		if(OutIndex >= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Helloo DestroyInstance"));
			InstancedMeshComponent->RemoveInstance(OutIndex);
			OnDestroyChildInstance(OutIndex, InLocation);
		}
	}
}

void ADigumWorldActorChild::DestroyInstance(const int32& InIndex)
{
	if(InIndex == INDEX_NONE) return;

	FTransform Transform;
	if(InstancedMeshComponent->GetInstanceTransform(InIndex, Transform, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("Helloo DestroyInstance %s"), *Transform.GetLocation().ToString());
		InstancedMeshComponent->RemoveInstance(InIndex);
		OnDestroyChildInstance(InIndex, Transform.GetLocation());
		
	}
}

void ADigumWorldActorChild::OnInteract_Implementation(const AActor* InInstigator,
	const FDigumWorldRequestParams& InParams)
{
	UE_LOG(LogTemp, Warning, TEXT("HitInstanceIndex: %i, %f"), InParams.HitInstanceIndex, InParams.Magnitude);
	if(InParams.Request == EDigumWorld_Request::DigumWorldRequest_Destroy)
	{
		DestroyInstance(InParams.HitInstanceIndex);
	}
}




