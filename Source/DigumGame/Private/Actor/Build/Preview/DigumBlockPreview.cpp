// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Build/Preview/DigumBlockPreview.h"


// Sets default values
ADigumBlockPreview::ADigumBlockPreview()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumBlockPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumBlockPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

