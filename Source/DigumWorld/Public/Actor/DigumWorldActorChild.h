// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumActor.h"
#include "GameFramework/Actor.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Subsystem/DigumWorldSubsystem.h"
#include "DigumWorldActorChild.generated.h"

struct FDigumWorldBlockID;
struct FDigumWorldProceduralCoordinateArray;
struct FDigumWorldAssetCoordinateArray;
class UDigumWorldSwatchAsset;
class UDigumWorldISMComponent;

USTRUCT()
struct FDigumWorldAsyncBlockResult
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName BlockID;

	UPROPERTY()
	int32 Variant;
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	FDigumWorldProceduralCoordinate Coordinate;

	UPROPERTY()
	FDigumWorldRequestParams RequestParams;
};

USTRUCT()
struct FDigumWorldAsyncBlockResultArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDigumWorldAsyncBlockResult> ResultArray;

	void Add(const FDigumWorldAsyncBlockResult& InResult)
	{
		ResultArray.Add(InResult);
	} 
};

UCLASS()
class DIGUMWORLD_API ADigumWorldActorChild : public ADigumActor, public IIDigumWorldInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDigumWorldISMComponent> InstancedMeshComponent;

	
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<float> Health;

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 SectionWidth = 0;

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 SectionHeight = 0;

public:
	// Sets default values for this actor's properties
	ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDigumWorldTransact, ADigumWorldActorChild*, const FDigumWorldProceduralSection&, const FDigumWorldRequestParams&)

	FOnDigumWorldTransact OnDigumWorldTransact;

	UPROPERTY(Replicated)
	bool bIsBlocking = false;
	
	UPROPERTY(Replicated)
	FName BlockID;
	
	UPROPERTY(Replicated)
	TObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;
	
	UPROPERTY(Replicated)
	FVector GridSize;

	UPROPERTY(Replicated)
	FDigumWorldProceduralSection SectionData;
	
	virtual void BeginPlay() override;
	virtual void OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);
	virtual void BuildChildProperties(UDigumWorldSwatchAsset* InSwatchAsset);

	bool GetInstancedHitIndex(const FVector HitLocation, const float& InMaxRange, int32& OutIndex);
	virtual void OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation);

	void AsyncAddBlock();

	void Transact_Internal(const FDigumWorldRequestParams& InParams);

	
	virtual void OnBlockAdded(const FDigumWorldRequestParams& InParams, const FDigumWorldProceduralCoordinate& InCoordinate) {};
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	TQueue<TSharedPtr<FDigumWorldAsyncBlockResultArray>> AsyncBlockResultArrayQueue;

	virtual void InitializeISMComponent(const int32& InHierarchyCount, const int32& InSectionWidth, const int32& InSectionHeight);
	virtual void InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates, const int32 HierarchyIndex = 0);
	virtual void InitializeSwatchAsset(const FName& InBlockID, UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldProceduralCoordinateArray Coordinates, const int32& NumOfHierarchies, const int32& InSectionWidth, const int32& InSectionHeight);
	virtual void InitializeSwatchAsset_UsingParams(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldProceduralCoordinateArray Coordinates, const int32& NumOfHierarchies, const int32& InSectionWidth, const int32& InSectionHeight, const FDigumWorldRequestParams& InParams);
	
	void ResetChildActor();
	virtual void SetWorldCollision(const bool& bValue);
	virtual void AddBlock(const FName& InBlockID, FDigumWorldProceduralCoordinateArray& InCoordinates);
	virtual void AddBlock(const FDigumWorldRequestParams& InParams, FDigumWorldProceduralCoordinateArray& InCoordinates);
	void OnCollide(AActor* InInstigator, const FVector& InLocation, const int32& InIndex = INDEX_NONE);
	void DestroyInstance(const FVector& InLocation, const float& InMaxRange);
	virtual void DestroyInstance(const int32& InIndex = INDEX_NONE);
	virtual void OnInteract_Implementation(const AActor* InInstigator, const FDigumWorldRequestParams& InParams) override;
	
	virtual void SetSectionData(FDigumWorldProceduralSection& InSection);
	virtual void RemoveBlock(const int32& InstanceIndex, const float& InScaledDamage);
	
	FORCEINLINE UDigumWorldISMComponent* GetInstancedMeshComponent() const { return InstancedMeshComponent; }
	FOnDigumWorldTransact& GetOnDigumWorldTransactDelegate() { return OnDigumWorldTransact; }
};
