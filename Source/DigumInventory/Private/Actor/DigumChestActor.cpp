// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumChestActor.h"

#include "Components/DigumInventoryComponent.h"


// Sets default values
ADigumChestActor::ADigumChestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UDigumInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->InitProperties.InventorySize = 10;
}

// Called when the game starts or when spawned
void ADigumChestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UDigumInventoryComponent* ADigumChestActor::GetInventoryComponent() const
{
	return InventoryComponent;
}

