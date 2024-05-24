// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumActionComponent.generated.h"

class UDigumAction;
DECLARE_LOG_CATEGORY_EXTERN(LogDigumActionComponent, Log, All);

struct FDigumActionProperties;

DECLARE_DELEGATE(FOnActionBeginExecuting)
DECLARE_DELEGATE(FOnActionFinishedExecuting)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMACTION_API UDigumActionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UDigumAction*> ActionPool;

	/*UPROPERTY()
	UDigumAction* BlockingAction;*/
	
	UPROPERTY()
	TArray<UDigumAction*> BlockingActions;
	
	
	UPROPERTY()
	float ActionsCheckInterval = 0.1f;
public:
	// Sets default values for this component's properties
	UDigumActionComponent();
	
protected:
	// Handle for cleaning up actions instead of using Tick
	FTimerHandle ActionsTimerHandler;

public:
	void CheckActions();
	void TryExecuteAction(const FDigumActionProperties& Properties);
	void TryExecuteAction(const FDigumActionProperties& Properties, FOnActionBeginExecuting& OnActionBeginExecuting, FOnActionFinishedExecuting& OnActionFinishedExecuting);

};
