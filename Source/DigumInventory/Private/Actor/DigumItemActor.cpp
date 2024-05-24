// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumItemActor.h"

ADigumItemActor::ADigumItemActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	PrimaryActorTick.bCanEverTick = false;
}
