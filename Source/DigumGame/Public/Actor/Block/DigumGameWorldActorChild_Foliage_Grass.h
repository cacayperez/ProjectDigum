// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumWorldActorChild.h"
#include "DigumGameWorldActorChild_Foliage_Grass.generated.h"

UCLASS()
class DIGUMGAME_API ADigumGameWorldActorChild_Foliage_Grass : public ADigumWorldActorChild
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumGameWorldActorChild_Foliage_Grass(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
