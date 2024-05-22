// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldActorChild.generated.h"

class UDigumWorldISMComponent;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorChild : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumWorldISMComponent> InstancedMeshComponent;
public:
	// Sets default values for this actor's properties
	ADigumWorldActorChild();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
