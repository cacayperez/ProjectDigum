// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumWorldSwatchAsset.h"
#include "Actor/DigumWorldActorChild.h"
#include "Asset/DigumAssetManager.h"

UMaterialInstance* UDigumWorldSwatchAsset::GetEditorMaterial() const
{
	return UDigumAssetManager::GetAsset<UMaterialInstance>(EditorMaterial);
}

TSubclassOf<ADigumWorldActorChild> UDigumWorldSwatchAsset::GetChildActorClass() const
{
	return UDigumAssetManager::GetSubclass<ADigumWorldActorChild>(ChildActorClass);
}

UMaterialInstance* UDigumWorldSwatchAsset::GetWorldMaterial() const
{
	return UDigumAssetManager::GetAsset<UMaterialInstance>(WorldMaterial);
}

UStaticMesh* UDigumWorldSwatchAsset::GetSwatchMesh() const
{
	return UDigumAssetManager::GetAsset<UStaticMesh>(SwatchMesh);
}

FVector UDigumWorldSwatchAsset::GetPositionOffset() const
{
	return PositionOffset;
}
