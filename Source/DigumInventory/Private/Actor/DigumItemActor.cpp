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

ADigumItemActor::~ADigumItemActor()
{
	UE_LOG(LogTemp, Warning, TEXT("ADigumItemActor::~ADigumItemActor"));
}

void ADigumItemActor::SetItemInstigator(AActor* InInstigator)
{
	ItemInstigator = InInstigator;
}

void ADigumItemActor::SetItemProperties(const FDigumItemProperties& InItemProperties)
{
	ItemProperties = InItemProperties;
}

FDigumItemProperties* ADigumItemActor::GetItemProperties()
{
	return &ItemProperties;
}
