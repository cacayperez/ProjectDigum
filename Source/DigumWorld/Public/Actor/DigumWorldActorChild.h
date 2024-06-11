// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumActor.h"
#include "GameFramework/Actor.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "DigumWorldActorChild.generated.h"

struct FDigumWorldProceduralCoordinateArray;
struct FDigumWorldAssetCoordinateArray;
class UDigumWorldSwatchAsset;
class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorChild : public ADigumActor, public IIDigumWorldInteractionInterface
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY()
	TObjectPtr<UDigumWorldISMComponent> InstancedMeshComponent;

	
	UPROPERTY()
	TArray<float> Health;

public:
	// Sets default values for this actor's properties
	ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY()
	FName BlockID = NAME_None;
	
	UPROPERTY()
	TObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;
	
	UPROPERTY()
	FVector GridSize;
	
	virtual void BeginPlay() override;
	virtual void OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);
	virtual void BuildChildProperties(UDigumWorldSwatchAsset* InSwatchAsset);

	bool GetInstancedHitIndex(const FVector HitLocation, const float& InMaxRange, int32& OutIndex);
	virtual void OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation);
	
public:
	virtual void InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates, const int32 HierarchyIndex = 0);
	virtual void InitializeSwatchAsset(const FName& InBlockID, UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldProceduralCoordinateArray Coordinates);

	virtual void SetWorldCollision(const bool& bValue);
	virtual void AddBlock(FDigumWorldProceduralCoordinateArray& InCoordinates);
	void OnCollide(AActor* InInstigator, const FVector& InLocation, const int32& InIndex = INDEX_NONE);
	void DestroyInstance(const FVector& InLocation, const float& InMaxRange);
	virtual void DestroyInstance(const int32& InIndex = INDEX_NONE);
	virtual void OnInteract_Implementation(const AActor* InInstigator, const FDigumWorldRequestParams& InParams) override;
	// void DestroyInstance(const FVector& InLocation, const int32& InIndex = INDEX_NONE);

	FORCEINLINE UDigumWorldISMComponent* GetInstancedMeshComponent() const { return InstancedMeshComponent; }
};
