// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldSwatchAsset.h"
#include "DigumGameWorldBlockAsset.generated.h"

class ADigumPickupActor;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameWorldBlockAsset : public UDigumWorldSwatchAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ADigumPickupActor> PickupActorClass;
};
