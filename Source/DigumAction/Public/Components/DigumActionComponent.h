// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumActionComponent.generated.h"

class UDigumAction;
DECLARE_LOG_CATEGORY_EXTERN(LogDigumActionComponent, Log, All);

struct FDigumActionProperties;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMACTION_API UDigumActionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UDigumAction*> ActionPool;

	UPROPERTY()
	UDigumAction* BlockingAction;
	
	UPROPERTY()
	float ActionsCheckInterval = 0.5f;
public:
	// Sets default values for this component's properties
	UDigumActionComponent();
	
protected:
	FTimerHandle ActionsTimerHandler;
	
public:
	void CheckActions();
	void TryExecutionAction(const FDigumActionProperties& Properties);
};
