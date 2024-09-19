// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumChestActor.h"
#include "GameFramework/Actor.h"
#include "DigumCrafterActor.generated.h"

UCLASS()
class DIGUMINVENTORY_API ADigumCrafterActor : public ADigumChestActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumCrafterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
