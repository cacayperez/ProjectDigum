// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Notifies/DigumActionBeginNotify.h"

void UDigumActionBeginNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnDigumActionBeginNotify.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
