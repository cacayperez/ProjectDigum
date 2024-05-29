// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumActor.h"
#include "GameFramework/Actor.h"
#include "Interface/IDigumWorldInteractionInterface.h"
#include "DigumWorldActorChild.generated.h"

struct FDigumWorldAssetCoordinateArray;
class UDigumWorldSwatchAsset;
class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorChild : public ADigumActor, public IIDigumWorldInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumWorldISMComponent> InstancedMeshComponent;

	UPROPERTY()
	TObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;

	UPROPERTY()
	TArray<float> Health;
	
public:
	// Sets default values for this actor's properties
	ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;
	virtual void OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);

	bool GetInstancedHitIndex(const FVector HitLocation, const float& InMaxRange, int32& OutIndex);
public:
	virtual void InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);
	void OnCollide(AActor* InInstigator, const FVector& InLocation, const int32& InIndex = INDEX_NONE);
	void DestroyInstance(const FVector& InLocation, const float& InMaxRange);
	void DestroyInstance(const int32& InIndex = INDEX_NONE);
	virtual void OnInteract_Implementation(const AActor* InInstigator, const FDigumWorldRequestParams& InParams) override;
	// void DestroyInstance(const FVector& InLocation, const int32& InIndex = INDEX_NONE);
};
