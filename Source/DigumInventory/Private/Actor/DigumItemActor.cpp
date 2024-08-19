// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumItemActor.h"

#include "Net/UnrealNetwork.h"

ADigumItemActor::ADigumItemActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

ADigumItemActor::~ADigumItemActor()
{
	UE_LOG(LogTemp, Warning, TEXT("ADigumItemActor::~ADigumItemActor"));
}

void ADigumItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADigumItemActor, ItemProperties);
	DOREPLIFETIME(ADigumItemActor, PlayerController);
	DOREPLIFETIME(ADigumItemActor, SlotIndex);
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

void ADigumItemActor::SetPlayerController(ADigumPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

void ADigumItemActor::SetSlotIndex(const int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
}
