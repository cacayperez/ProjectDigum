// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Action/DigumGameAnimatedAction.h"

#include "Character/DigumCharacter.h"

DEFINE_LOG_CATEGORY(LogDigumAction);

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

void UDigumGameAnimatedAction::OnExecuteAction(AActor* InExecutor, UObject* InPayload)
{
	ADigumCharacter* DigumCharacter = Cast<ADigumCharacter>(InExecutor);
	if(AnimationMontage && DigumCharacter)
	{
		AnimInstance = DigumCharacter->GetMesh()->GetAnimInstance();
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
				UE_LOG(LogDigumAction, Error, TEXT("Failed to play animation montage"));
				EndAction(EDigumActionResult::DigumAction_Failed);
			}
		}
	}
}

void UDigumGameAnimatedAction::InitializeDefaults()
{
	bIsBlockingAction = true;
}
