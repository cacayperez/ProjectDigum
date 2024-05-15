// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/DigumGameActionBarComponent.h"


// Sets default values for this component's properties
UDigumGameActionBarComponent::UDigumGameActionBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDigumGameActionBarComponent::ActivateAction_Internal(const int32& ItemIndex)
{
	OnActivateItemAction.Broadcast(ItemIndex);
}

void UDigumGameActionBarComponent::InitializeActionKeys(const TArray<int32> ItemIndices)
{
	ActionKeys.Empty();
	for (int32 i = 0; i < ItemIndices.Num(); i++)
	{
		ActionKeys.Add(ItemIndices[i]);
	}

	if(ActionKeys.Num() > 0)
	{
		ActiveActionIndex = 0;
	}
}

void UDigumGameActionBarComponent::SetActionIndex(const int32& ActionIndex, const int32& ItemIndex)
{
	if(ActionKeys.IsValidIndex(ActionIndex))
	{
		ActionKeys[ActionIndex] = ItemIndex;
	}
}

void UDigumGameActionBarComponent::TryActivateAction(const int32& ActionIndex)
{
	if(ActionKeys.IsValidIndex(ActionIndex))
	{
		ActivateAction_Internal(ActionKeys[ActionIndex]);
	}
}

void UDigumGameActionBarComponent::ActivateDefaultAction()
{
	TryActivateAction(ActiveActionIndex);
}

void UDigumGameActionBarComponent::SetActiveAction(const int32& Index)
{
	ActiveActionIndex = Index;
}
