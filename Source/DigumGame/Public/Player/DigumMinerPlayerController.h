// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/DigumPlayerController.h"
#include "DigumMinerPlayerController.generated.h"

UCLASS()
class DIGUMGAME_API ADigumMinerPlayerController : public ADigumPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumMinerPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
