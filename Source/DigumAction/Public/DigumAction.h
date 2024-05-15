// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumAction.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDigumAction, Log, All);
/**
 * 
 */
UCLASS()
class DIGUMACTION_API UDigumAction : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	bool bFinishedExecuting = false;

protected:

	DECLARE_DELEGATE(FOnFinishedAction);
	FOnFinishedAction OnFinishedAction;
	
	UPROPERTY()
	bool bIsBlockingAction = false;
	
	enum EDigumActionResult : uint8
	{
		DigumAction_Failed,
		DigumAction_Success,
	};
	
	virtual void OnExecuteAction(AActor* InExecutor);
	virtual void OnEndAction(EDigumActionResult Result);
	
public:
	virtual void InitializeDefaults();
	void ExecuteAction(AActor* InExecutor);
	void EndAction(EDigumActionResult Result);

	bool IsFinishedExecuting() const;
	bool IsBlockingAction() const;
	
	FOnFinishedAction& GetOnFinishedAction() { return OnFinishedAction;}
};
