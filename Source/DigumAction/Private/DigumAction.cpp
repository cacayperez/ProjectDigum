// Fill out your copyright notice in the Description page of Project Settings.


#include "DigumAction.h"

DEFINE_LOG_CATEGORY(LogDigumAction);

void UDigumAction::OnExecuteAction(AActor* InExecutor)
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

void UDigumAction::ExecuteAction(AActor* InExecutor)
{
	OnExecuteAction(InExecutor);
}

void UDigumAction::EndAction(EDigumActionResult Result)
{
	OnEndAction(Result);
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
