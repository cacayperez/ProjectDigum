// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Components/DigumActionComponent.h"
#include "DigumAction.h"
#include "Properties/DigumActionProperties.h"

DEFINE_LOG_CATEGORY(LogDigumActionComponent);

// Sets default values for this component's properties
UDigumActionComponent::UDigumActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDigumActionComponent::CheckActions()
{
	if(ActionPool.Num() > 0)
	{
		for(int32 i = 0; i < ActionPool.Num(); i++)
		{
			UDigumAction* Action = ActionPool[i];
			if(Action && Action->IsFinishedExecuting())
			{
				ActionPool.RemoveAt(i);
			}
		}
	}
	if(ActionPool.Num() <= 0 && BlockingActions.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionsTimerHandler);
	}
	
	if(!BlockingActions.IsEmpty())
	{
		UDigumAction* Action = BlockingActions[0];
		if(Action && Action->IsFinishedExecuting())
		{
			Action->ConditionalBeginDestroy();
			BlockingActions.RemoveAt(0);
		}
	}

	if(GEngine) GEngine->ForceGarbageCollection();
}

void UDigumActionComponent::TryExecuteAction(const FDigumActionProperties& Properties)
{
	if(Properties.ActionClass && Properties.ActionInstigator)
	{
		if(UDigumAction* Action = NewObject<UDigumAction>(this, Properties.ActionClass))
		{
			Action->InitializeDefaults();

			if(!Action->IsBlockingAction())
			{
				ActionPool.Add(Action);
				Action->ExecuteAction(Properties.ActionInstigator);
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Added to Pool: %s, Num : %i"), *Properties.ActionClass->GetName(), ActionPool.Num());
			}
			
			// Set Blocking Action
			// There can only be one blocking action
			if(Action->IsBlockingAction() && BlockingActions.IsEmpty())
			{
				BlockingActions.Add(Action);
				Action->ExecuteAction(Properties.ActionInstigator);
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Blocking: %s"), *Properties.ActionClass->GetName());
			}
		}
	}
	else
	{
		UE_LOG(LogDigumActionComponent, Error, TEXT("Failed Action"));
	}

	if(ActionPool.Num() > 0 || BlockingActions.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ActionsTimerHandler, this, &UDigumActionComponent::CheckActions, ActionsCheckInterval, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionsTimerHandler);
	}
}


void UDigumActionComponent::TryExecuteAction(const FDigumActionProperties& Properties, FOnActionBeginExecuting& OnActionBeginExecuting, FOnActionFinishedExecuting& OnActionFinishedExecuting)
{
	if(Properties.ActionClass && Properties.ActionInstigator)
	{
		if(UDigumAction* Action = NewObject<UDigumAction>(this, Properties.ActionClass))
		{
			Action->InitializeDefaults();
			Action->GetOnBeginExecuteAction().BindLambda([this, Action, OnActionBeginExecuting]()
			{
				OnActionBeginExecuting.ExecuteIfBound();
			});

			Action->GetOnFinishedAction().BindLambda([this, Action, OnActionFinishedExecuting]()
			{
				OnActionFinishedExecuting.ExecuteIfBound();
			});

			if(!Action->IsBlockingAction())
			{
				ActionPool.Add(Action);
				Action->ExecuteAction(Properties.ActionInstigator);
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Added to Pool: %s, Num : %i"), *Properties.ActionClass->GetName(), ActionPool.Num());
			}
			
			// Set Blocking Action
			// There can only be one blocking action
			if(Action->IsBlockingAction() && BlockingActions.IsEmpty())
			{
				BlockingActions.Add(Action);
				Action->ExecuteAction(Properties.ActionInstigator);
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Blocking: %s"), *Properties.ActionClass->GetName());
			}
		}
	}
	else
	{
		UE_LOG(LogDigumActionComponent, Error, TEXT("Failed Action"));
	}

	if(ActionPool.Num() > 0 || BlockingActions.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ActionsTimerHandler, this, &UDigumActionComponent::CheckActions, ActionsCheckInterval, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionsTimerHandler);
	}
}
