// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorChild.h"


// Sets default values
ADigumWorldActorChild::ADigumWorldActorChild()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumWorldActorChild::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumWorldActorChild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

