// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DigumActionBeginNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDigumActionBeginNotify);

/**
 * 
 */
UCLASS()
class DIGUMACTION_API UDigumActionBeginNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	FOnDigumActionBeginNotify OnDigumActionBeginNotify;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
