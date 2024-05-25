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

public:
	~UDigumAction();

	virtual void FinishDestroy() override;
protected:
	UPROPERTY()
	bool bFinishedExecuting = false;
	
	DECLARE_DELEGATE(FOnBeginExecuteActionDelegate);
	DECLARE_DELEGATE(FOnFinishExecuteActionDelegate);
	
	FOnFinishExecuteActionDelegate OnFinishedAction;
	FOnBeginExecuteActionDelegate OnBeginExecuteAction;
	
	UPROPERTY()
	bool bIsBlockingAction = false;
	
	enum EDigumActionResult : uint8
	{
		DigumAction_Failed,
		DigumAction_Success,
	};
	
	virtual void OnExecuteAction(AActor* InExecutor, UObject* InPayload = nullptr);
	virtual void OnEndAction(EDigumActionResult Result);
public:
	virtual void InitializeDefaults();
	virtual void ExecuteAction(AActor* InExecutor, UObject* InPayload = nullptr);
	virtual void EndAction(EDigumActionResult Result);

	bool IsFinishedExecuting() const;
	virtual bool IsBlockingAction() const;
	
	FOnFinishExecuteActionDelegate& GetOnFinishedAction() { return OnFinishedAction;}
	FOnBeginExecuteActionDelegate& GetOnBeginExecuteAction() { return OnBeginExecuteAction;}
};
