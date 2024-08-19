// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumGameEquipComponent.generated.h"


class ADigumPlayerController;
struct FDigumItemProperties;
class ADigumItemActor;

UENUM()
enum EDigumGame_EquipSlot : uint8
{
	DigumEquipSlot_Default,
	DigumEquipSlot_MainHand,
	DigumEquipSlot_OffHand,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameEquipComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Digum Inventory", meta=(AllowPrivateAccess="true"))
	TMap<TEnumAsByte<EDigumGame_EquipSlot>, TObjectPtr<ADigumItemActor>> EquippedItems;

	UPROPERTY()
	TObjectPtr<ADigumPlayerController> PlayerController = nullptr;
	
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

private:
	void SetEquippedItemActor(const EDigumGame_EquipSlot EquipSlot, ADigumItemActor* ItemActor);
	
public:
	void EquipItem(const TSubclassOf<ADigumItemActor> ItemActorClass, const FDigumItemProperties& InItemProperties, const int32 InSlotIndex = -1, const EDigumGame_EquipSlot EquipSlot = EDigumGame_EquipSlot::DigumEquipSlot_MainHand);
	// ADigumItemActor*& GetEquippedItemActor() const { return EquippedItemActor; }
	
	ADigumItemActor* GetEquippedItemActor(const EDigumGame_EquipSlot EquipSlot);
	void ClearEquippedItem(const EDigumGame_EquipSlot EquipSlot = EDigumGame_EquipSlot::DigumEquipSlot_MainHand);

	ADigumPlayerController* GetPlayerController() const { return PlayerController; }
	void SetPlayerController(ADigumPlayerController* InPlayerController);
};
