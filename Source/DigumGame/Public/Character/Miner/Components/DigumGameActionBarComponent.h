// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DigumGameEquipComponent.h"
#include "Actor/DigumGameItemActor_ActiveItem.h"
#include "Components/ActorComponent.h"
#include "DigumGameActionBarComponent.generated.h"


USTRUCT()
struct FDigumGameActionParams
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 ItemIndex = INDEX_NONE;
	
	UPROPERTY()
	TEnumAsByte<EDigumGame_EquipSlot> EquipSlot = EDigumGame_EquipSlot::DigumEquipSlot_Default;

	UPROPERTY()
	TEnumAsByte<EDigumGameItem_ActionKey> ActionKey = EDigumGameItem_ActionKey::DigumGameActionKey_Default;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UDigumGameActionBarComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> ActionKeys;

	UPROPERTY()
	int32 ActionKeysNum = 10;

	UPROPERTY()
	int32 ActiveActionIndex = -1;

	UPROPERTY()
	TEnumAsByte<EDigumGame_EquipSlot> EquipSlot = EDigumGame_EquipSlot::DigumEquipSlot_MainHand;
	
public:
	// Sets default values for this component's properties
	UDigumGameActionBarComponent();
	
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateItemAction, const FDigumGameActionParams&, InActionParams);

	FOnActivateItemAction OnActivateItemAction;
private:
	void ActivateAction_Internal(const FDigumGameActionParams& InActionParams);

public:
	void InitializeActionKeys(const TArray<int32> ItemIndices);
	void SetActionIndex(const int32& ActionIndex, const int32& ItemIndex);
	void SetActiveAction(const int32& ActionIndex);
	void TryActivateAction(const FDigumGameActionParams& InActionParams);
	void ActivatePrimaryAction();
	
	

	FOnActivateItemAction& OnActivateItemActionDelegate() { return OnActivateItemAction; }
};
