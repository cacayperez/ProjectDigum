// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldLayer.generated.h"

UENUM()
enum EDigumWorldLayerCollisionType
{
	None,
	Overlap,
	Block,
};

USTRUCT(BlueprintType)
struct FDigumWorldLayer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName LayerName;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDigumWorldLayerCollisionType> CollisionType;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> LayerMesh;
	
	UPROPERTY(EditAnywhere)
	UMaterialInstance* LayerMaterial;

	UStaticMesh* GetLayerMesh() const { return LayerMesh.Get(); }
};
