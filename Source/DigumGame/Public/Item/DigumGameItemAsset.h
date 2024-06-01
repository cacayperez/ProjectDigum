// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumItemAsset.h"
#include "UObject/Object.h"
#include "DigumGameItemAsset.generated.h"

class UDigumAction;
class ADigumBuildPreviewActor;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameItemAsset : public UDigumItemAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UDigumAction> Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> BuildPreviewActorClass;

	TSubclassOf<AActor> GetBuildPreviewActorClass() const;
};
