// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumAvatarModifierActor.h"


// Sets default values
ADigumAvatarModifierActor::ADigumAvatarModifierActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumAvatarModifierActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumAvatarModifierActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

