// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Actor/DigumGameItemActor_ActiveItem.h"

#include "DigumAction.h"
#include "Asset/DigumAssetManager.h"
#include "Components/DigumActionComponent.h"
#include "Interface/IDigumActionComponentInterface.h"
#include "Properties/DigumActionProperties.h"

DEFINE_LOG_CATEGORY(LogDigumGameItemActor_ActiveItem);

ADigumGameItemActor_ActiveItem::ADigumGameItemActor_ActiveItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADigumGameItemActor_ActiveItem::TryActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	OnActivateItem(InInstigator, ActionKey);
}

void ADigumGameItemActor_ActiveItem::OnActivateItem(AActor* InInstigator, const EDigumGameItem_ActionKey ActionKey)
{
	UDigumActionComponent* ActionComponent = nullptr;
	if(InInstigator->GetClass()->ImplementsInterface(UIDigumActionComponentInterface::StaticClass()))
	{
		ActionComponent = IIDigumActionComponentInterface::Execute_GetActionComponentBP(InInstigator);
	}

	if(ActionComponent == nullptr) return;
	
	ItemInstigator = InInstigator;
	if(InInstigator && Actions.Contains(ActionKey))
	{
		FDigumActionProperties ActionProperties;
		ActionProperties.ActionInstigator = InInstigator;
		ActionProperties.ActionClass = GetActionClass(ActionKey);
		
		FOnActionBeginExecuting ActionBeginDelegate;
		FOnActionFinishedExecuting ActionFinishedDelegate;
		
		ActionFinishedDelegate.BindLambda([this, InInstigator, ActionKey]()
		{
			OnActionFinished(InInstigator, ActionKey);
		});

		ActionBeginDelegate.BindLambda([this, InInstigator, ActionKey]()
		{
			
			OnActionBegin(InInstigator, ActionKey);
		});
		
		ActionComponent->TryExecuteAction(ActionProperties, ActionBeginDelegate, ActionFinishedDelegate);

	}
}

TSubclassOf<UDigumAction> ADigumGameItemActor_ActiveItem::GetActionClass(const EDigumGameItem_ActionKey ActionKey) const
{
	if(Actions.Contains(ActionKey))
	{
		return UDigumAssetManager::GetSubclass<UDigumAction>(Actions[ActionKey]);
	}
	return nullptr;
}

void ADigumGameItemActor_ActiveItem::OnActionBegin(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	
}

void ADigumGameItemActor_ActiveItem::OnActionFinished(AActor* InInstigator, const EDigumGameItem_ActionKey& ActionKey)
{
	
}



