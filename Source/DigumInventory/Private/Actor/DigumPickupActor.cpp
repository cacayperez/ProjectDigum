// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumPickupActor.h"


// Sets default values
ADigumPickupActor::ADigumPickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADigumPickupActor::OnPickup(AActor* InPickupInstigator)
{
}
