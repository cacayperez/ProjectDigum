// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Item/DigumGameItemAsset.h"

TSubclassOf<AActor> UDigumGameItemAsset::GetBuildPreviewActorClass() const
{
	return UDigumAssetManager::GetSubclass<AActor>(BuildPreviewActorClass);
}
