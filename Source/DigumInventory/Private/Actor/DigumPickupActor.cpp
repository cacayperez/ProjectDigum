// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumPickupActor.h"

#include "Components/DigumInventoryComponent.h"
#include "Interface/IDigumInventoryInterface.h"


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
	if(InPickupInstigator && InPickupInstigator->GetClass()->ImplementsInterface(UIDigumInventoryInterface::StaticClass()))
	{
		if(const TScriptInterface<IIDigumInventoryInterface> Interface = InPickupInstigator)
		{
			if(UDigumInventoryComponent* InventoryComponent = Interface->GetInventoryComponent())
			{
				int32 Excess = 0;
				UE_LOG(LogTemp, Warning, TEXT("ADigumPickupActor::OnPickup, %s"), *ItemProperties.GetItemID().ToString());
				InventoryComponent->TryAddItem(ItemProperties, Excess);
				if(Excess <= 0)
				{
					Destroy();
				}
			}
			
		}
	}
}

void ADigumPickupActor::SetItemProperties(const FDigumItemProperties& InItemProperties)
{
	ItemProperties = InItemProperties;
}
