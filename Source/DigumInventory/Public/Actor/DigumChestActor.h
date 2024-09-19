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

	UPROPERTY()
	int32 KeyID = -1;
	
public:
	// Sets default values for this actor's properties
	ADigumChestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TryOpen(const int32 KeyID);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UDigumInventoryComponent* GetInventoryComponent() const override;
	virtual void Open(int32 KeyID = -1) override;
	virtual void Close() override;
	virtual void Lock() override;
	virtual int32 GetKeyID() const override;

	
	
};
