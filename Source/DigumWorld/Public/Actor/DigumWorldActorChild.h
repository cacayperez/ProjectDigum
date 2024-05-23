// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumActor.h"
#include "GameFramework/Actor.h"
#include "DigumWorldActorChild.generated.h"

struct FDigumWorldAssetCoordinateArray;
class UDigumWorldSwatchAsset;
class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorChild : public ADigumActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumWorldISMComponent> InstancedMeshComponent;

	UPROPERTY()
	TObjectPtr<UDigumWorldSwatchAsset> SwatchAsset;
	
public:
	// Sets default values for this actor's properties
	ADigumWorldActorChild(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void OnFinishedInitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);

public:
	virtual void InitializeSwatchAsset(UDigumWorldSwatchAsset* InSwatchAsset, FDigumWorldAssetCoordinateArray Coordinates);
	
};
