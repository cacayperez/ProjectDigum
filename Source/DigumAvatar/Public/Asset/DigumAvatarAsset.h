// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DigumAvatarAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMAVATAR_API UDigumAvatarAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY()
	TSoftObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY()
	TSoftObjectPtr<UMaterialInstance> MaterialInstance;
};
