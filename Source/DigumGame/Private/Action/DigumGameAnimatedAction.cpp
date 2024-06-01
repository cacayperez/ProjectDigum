// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Action/DigumGameAnimatedAction.h"

#include "Character/DigumCharacter.h"
#include "Notifies/DigumActionBeginNotify.h"
#include "Notifies/DigumActionEndNotify.h"


UDigumGameAnimatedAction::~UDigumGameAnimatedAction()
{
	AnimationMontage = nullptr;
	AnimInstance = nullptr;
}

void UDigumGameAnimatedAction::OnMontageEnded(UAnimMontage* AnimMontage, bool bArg)
{
	MontageEndDelegate.Unbind();
	EndAction(EDigumActionResult::DigumAction_Success);
}

void UDigumGameAnimatedAction::OnBeginAnimationActionNotify()
{
	OnBeginExecuteAction.ExecuteIfBound();
}

void UDigumGameAnimatedAction::OnEndAnimationActionNotify()
{
	OnFinishedAction.ExecuteIfBound();
}

void UDigumGameAnimatedAction::OnExecuteAction(AActor* InExecutor, UObject* InPayload)
{
	ADigumCharacter* DigumCharacter = Cast<ADigumCharacter>(InExecutor);
	if(AnimationMontage && DigumCharacter)
	{
		for(const auto Notify : AnimationMontage->Notifies)
		{
			if(const auto ActionBegin = Cast<UDigumActionBeginNotify>(Notify.Notify))
			{
				ActionBegin->OnDigumActionBeginNotify.AddUObject(this, &UDigumGameAnimatedAction::OnBeginAnimationActionNotify);
			}
			if(const auto ActionEnd = Cast<UDigumActionEndNotify>(Notify.Notify))
			{
				ActionEnd->OnDigumActionEndNotify.AddUObject(this, &UDigumGameAnimatedAction::OnEndAnimationActionNotify);
			}
		}
		
		AnimInstance = DigumCharacter->GetMesh()->GetAnimInstance();
		// DigumCharacter->GetAni
		if(AnimInstance)
		{
			float Result = AnimInstance->Montage_Play(AnimationMontage);
			if(Result >= 0)
			{
				MontageEndDelegate.BindUObject(this, &UDigumGameAnimatedAction::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AnimationMontage);
			}
			else
			{
				// UE_LOG(LogDigumAction, Error, TEXT("Failed to play animation montage"));
				EndAction(EDigumActionResult::DigumAction_Failed);
			}
		}
	}
}

void UDigumGameAnimatedAction::ExecuteAction(AActor* InExecutor, UObject* InPayload)
{
	OnExecuteAction(InExecutor, InPayload);
}

void UDigumGameAnimatedAction::EndAction(EDigumActionResult Result)
{
	
	OnEndAction(Result);
	
	bFinishedExecuting = true;
}

void UDigumGameAnimatedAction::InitializeDefaults()
{
	bIsBlockingAction = true;
}
