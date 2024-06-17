// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/Block/DigumGameWorldChild_Foliage_Tree.h"


// Sets default values
ADigumGameWorldChild_Foliage_Tree::ADigumGameWorldChild_Foliage_Tree(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumGameWorldChild_Foliage_Tree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumGameWorldChild_Foliage_Tree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

