// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DigumExternalStorageInterface.h"
#include "DigumChestActor.generated.h"

class UDigumInventoryComponent;

UCLASS()
class DIGUMINVENTORY_API ADigumChestActor : public AActor, public IDigumExternalStorageInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Digum Inventory")
	TObjectPtr<UDigumInventoryComponent> InventoryComponent;
	
public:
	// Sets default values for this actor's properties
	ADigumChestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UDigumInventoryComponent* GetInventoryComponent() const override;
};
