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
	PrimaryComponentTick.bCanEverTick = true;

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
	if(ActionPool.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionsTimerHandler);
	}
}

void UDigumActionComponent::TryExecutionAction(const FDigumActionProperties& Properties)
{
	if(Properties.ActionClass && Properties.ActionInstigator)
	{
		UDigumAction* Action = NewObject<UDigumAction>(this, Properties.ActionClass);
		if(Action)
		{
			
			Action->InitializeDefaults();

			// Set Blocking Action
			// There can only be one blocking action
			if(Action->IsBlockingAction())
			{
				if(BlockingAction == nullptr)
				{
					BlockingAction = Action;
					Action->GetOnFinishedAction().BindLambda([&]()
					{
						BlockingAction = nullptr;	
					});
					
					Action->ExecuteAction(Properties.ActionInstigator);
				}
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Blocking: %s"), *Properties.ActionClass->GetName());
			}
			else
			{
				Action->ExecuteAction(Properties.ActionInstigator);
				ActionPool.Add(Action);
				UE_LOG(LogDigumActionComponent, Log, TEXT("Action Added to Pool: %s, Num : %i"), *Properties.ActionClass->GetName(), ActionPool.Num());
			}

		}

	}
	else
	{
		UE_LOG(LogDigumActionComponent, Error, TEXT("Failed Action"));
	}

	if(ActionPool.Num() > 0 || BlockingAction)
	{
		GetWorld()->GetTimerManager().SetTimer(ActionsTimerHandler, this, &UDigumActionComponent::CheckActions, ActionsCheckInterval, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionsTimerHandler);
	}
}
