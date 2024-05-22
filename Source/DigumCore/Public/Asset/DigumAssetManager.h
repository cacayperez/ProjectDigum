// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DigumAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMCORE_API UDigumAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UDigumAssetManager();

	// Return Singleton RPG AssetManager
	static UDigumAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetPrimaryAsset(const FPrimaryAssetId& AssetId);
	
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
	
	static UObject* GetObjectWithPrimaryID(const FPrimaryAssetId& AssetId);
	
	// Logs all assets currently loaded and tracked by the asset manager.
	static void DumpLoadedAssets();

protected:
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);

private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UDigumAssetManager::GetPrimaryAsset(const FPrimaryAssetId& AssetId)
{
	AssetType* LoadedAsset = nullptr;
	//FAssetData AssetData;
	if(UObject* AssetObject = Get().GetPrimaryAssetObject(AssetId))
	{
		LoadedAsset = Cast<AssetType>(AssetObject);
		
	}
	return LoadedAsset;
}

template <typename AssetType>
AssetType* UDigumAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if(AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if(!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UDigumAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if(AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if(!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}
		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}