// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumAssetManager.h"
#include "Actor/DigumItemActor.h"
#include "Engine/DataAsset.h"
#include "DigumItemAsset.generated.h"

class ADigumPickupActor;
/**
 * 
 */
UCLASS()
class DIGUMINVENTORY_API UDigumItemAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> DisplayTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> DisplayMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ADigumItemActor> ItemActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ADigumPickupActor> PickupActorClass;
	
	UTexture2D* GetItemTexture() const { return UDigumAssetManager::GetAsset<UTexture2D>(DisplayTexture); }
	UMaterialInterface* GetItemMaterial() const { return UDigumAssetManager::GetAsset<UMaterialInterface>(DisplayMaterial); }
	TSubclassOf<ADigumItemActor> GetItemActorClass() const { return UDigumAssetManager::GetSubclass<ADigumItemActor>(ItemActorClass); }
};
