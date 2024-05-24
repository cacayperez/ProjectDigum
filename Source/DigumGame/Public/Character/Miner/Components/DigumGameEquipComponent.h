// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumGameEquipComponent.generated.h"


class ADigumItemActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameEquipComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	TSubclassOf<ADigumItemActor> EquippedItemActorClass;

	UPROPERTY()
	TObjectPtr<ADigumItemActor> EquippedItemActor = nullptr;
	
public:
	// Sets default values for this component's properties
	UDigumGameEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EquipItem(const TSubclassOf<ADigumItemActor> ItemActorClass);
};
