// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DigumPickupInterface.h"
#include "DigumPickupActor.generated.h"

UCLASS()
class DIGUMINVENTORY_API ADigumPickupActor : public AActor, public IDigumPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickup(AActor* InPickupInstigator) override;
};
