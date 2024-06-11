// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DigumWorldActorChild.h"
#include "DigumGameWorldChild_Foliage_Tree.generated.h"

UCLASS()
class DIGUMGAME_API ADigumGameWorldChild_Foliage_Tree : public ADigumWorldActorChild
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumGameWorldChild_Foliage_Tree(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
