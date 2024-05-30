// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumWorldProceduralAsset.h"

#include "Asset/DigumAssetManager.h"
#include "Asset/DigumWorldSwatchAsset.h"

UDigumWorldSwatchAsset* UDigumWorldProceduralAsset::GetSwatchAsset(const FName& BlockID)
{
	for(const auto& Block : Blocks)
	{
		if(Block.BlockID == BlockID)
		{
			return UDigumAssetManager::GetAsset<UDigumWorldSwatchAsset>(Block.SwatchAsset);
		}
	}
	return nullptr;
}
