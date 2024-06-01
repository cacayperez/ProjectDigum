// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumWorldActorChild_GridSquare.h"

ADigumWorldActorChild_GridSquare::ADigumWorldActorChild_GridSquare(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADigumWorldActorChild_GridSquare::OnDestroyChildInstance(const int32& InIndex, const FVector& InLocation)
{
	
}

void ADigumWorldActorChild_GridSquare::SpawnBlockPickup(const FVector& InLocation)
{
}
