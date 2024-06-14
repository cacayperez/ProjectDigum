// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Block/DigumGameWorldActorChild_Foliage_Grass.h"

#include "Components/DigumWorldISMComponent.h"


// Sets default values
ADigumGameWorldActorChild_Foliage_Grass::ADigumGameWorldActorChild_Foliage_Grass(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumGameWorldActorChild_Foliage_Grass::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ADigumGameWorldActorChild_Foliage_Grass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

