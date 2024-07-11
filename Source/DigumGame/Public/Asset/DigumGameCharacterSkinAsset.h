// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumAssetManager.h"
#include "Engine/DataAsset.h"
#include "DigumGameCharacterSkinAsset.generated.h"

class UMaterialInterface;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameCharacterSkinAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText Name;

	UPROPERTY()
	FText Description;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Texture;

	UMaterialInterface* GetMaterialInterface()  const { return UDigumAssetManager::GetAsset<UMaterialInterface>(Material); }
	UTexture2D* GetTexture2D() const { return UDigumAssetManager::GetAsset<UTexture2D>(Texture); }
};
