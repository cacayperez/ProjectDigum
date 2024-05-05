// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/DigumMinerPlayerController.h"



// Sets default values
ADigumMinerPlayerController::ADigumMinerPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumMinerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

