// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumBlockPreview.generated.h"

UCLASS()
class DIGUMGAME_API ADigumBlockPreview : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumBlockPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
