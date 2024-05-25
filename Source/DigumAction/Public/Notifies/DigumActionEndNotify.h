// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DigumActionEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDigumActionEndNotify);

/**
 * 
 */
UCLASS()
class DIGUMACTION_API UDigumActionEndNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	FOnDigumActionEndNotify OnDigumActionEndNotify;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
