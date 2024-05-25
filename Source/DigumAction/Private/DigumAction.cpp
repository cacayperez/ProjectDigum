// Fill out your copyright notice in the Description page of Project Settings.


#include "DigumAction.h"

DEFINE_LOG_CATEGORY(LogDigumAction);

UDigumAction::~UDigumAction()
{
	OnBeginExecuteAction.Unbind();
	OnFinishedAction.Unbind();
}

void UDigumAction::FinishDestroy()
{
	UObject::FinishDestroy();
	OnBeginExecuteAction.Unbind();
	OnFinishedAction.Unbind();
}

void UDigumAction::OnExecuteAction(AActor* InExecutor, UObject* InPayload)
{
	// Override this function in child classes
	// Make sure to end action
	EndAction(DigumAction_Success);
}

void UDigumAction::OnEndAction(EDigumActionResult Result)
{
	
}

void UDigumAction::InitializeDefaults()
{
	bFinishedExecuting = false;
	bIsBlockingAction = false;
}

void UDigumAction::ExecuteAction(AActor* InExecutor, UObject* InPayload )
{
	OnBeginExecuteAction.ExecuteIfBound();
	OnExecuteAction(InExecutor, InPayload);
}

void UDigumAction::EndAction(EDigumActionResult Result)
{
	OnEndAction(Result);
	
	OnFinishedAction.ExecuteIfBound();
	
	bFinishedExecuting = true;
}

bool UDigumAction::IsFinishedExecuting() const
{
	return bFinishedExecuting;
}

bool UDigumAction::IsBlockingAction() const
{
	return bIsBlockingAction;
}
