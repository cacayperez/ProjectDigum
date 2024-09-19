// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumCrafterActor.h"


// Sets default values
ADigumCrafterActor::ADigumCrafterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumCrafterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumCrafterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

