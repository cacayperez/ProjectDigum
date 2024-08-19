// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumWorldActorChild.h"
#include "DigumGameWorldActorChild_Block.generated.h"

class ADigumPickupActor;
class UDigumGameWorldBlockAsset;

UCLASS()
class DIGUMGAME_API ADigumGameWorldActorChild_Block : public ADigumWorldActorChild
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumGameWorldBlockAsset> BlockAsset;

public:
	// Sets default values for this actor's properties
	ADigumGameWorldActorChild_Block(const FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation) override;
	virtual void BuildChildProperties(UDigumWorldSwatchAsset* InSwatchAsset) override;
	TSubclassOf<ADigumPickupActor> GetPickupActorClass() const;

};
