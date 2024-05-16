// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumWorldSwatch.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMWORLD_API UDigumWorldSwatch : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* EditorMaterial;
};
