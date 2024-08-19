// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldISMComponent.generated.h"

USTRUCT()
struct FDigumWorldISMInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 LocalIndex = -1;

	
	UPROPERTY()
	int32 Variant = -1;
	
	UPROPERTY()
	bool bSurfacePoint = false;
	
	UPROPERTY()
	FLinearColor Tint = FLinearColor::White;

	UPROPERTY()
	float Health = 1.0f;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	int32 HierarchyIndex = -1;

	UPROPERTY()
	FDigumWorldProceduralCoordinate Coordinate;

	void DecreaseHealth(const float& InAmount) { Health = Health - InAmount; }
	void IncreaseHealth(const float& InAmount) { Health = Health + InAmount; }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMWORLD_API UDigumWorldISMComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

	
	TMap<int32, TArray<TSharedPtr<FDigumWorldISMInstanceData>>> ISMInstanceData;

	UPROPERTY()
	TArray<FDigumWorldISMInstanceData> WorldISMData;
public:
	// Sets default values for this component's properties
	UDigumWorldISMComponent();
	
private:
	/*bool RemoveInstanceInternal_Custom(int32 InstanceIndex, bool InstanceAlreadyRemoved);*/
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeSize(const int32& InHierarchyCount, const int32& InSectionWidth, const int32& InSectionHeight);
	/*virtual bool RemoveInstance(int32 InstanceIndex) override;*/
	void SetTint(const int32& InstanceIndex, const int32& InHierarchyIndex);
	void SetSurfacePoint(const int32& InstanceIndex, const bool& bValue = false);
	void SetVariant(const int32& InstanceIndex, const int32& Variant);
	bool AddWorldInstance(const FTransform& InTransform, const FDigumWorldProceduralCoordinate& InCoordinate, const int32& InVariant, const int32& InLocalIndex, const bool& bHasTopNeighbor = true);
	bool RemoveWorldInstance(const int32& InInstanceIndex);
	void RemoveWorldInstance(const int32& InLocalX, const int32& InLocalY, const int32& InHierarchyIndex);
	
	// void RemoveWorldInstance(const int32& InstanceIndex);

	// int32 GetInstanceIndex(const int32& InLocalX, const int32) const;
};
