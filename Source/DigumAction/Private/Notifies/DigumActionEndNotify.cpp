// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Notifies/DigumActionEndNotify.h"

void UDigumActionEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnDigumActionEndNotify.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
