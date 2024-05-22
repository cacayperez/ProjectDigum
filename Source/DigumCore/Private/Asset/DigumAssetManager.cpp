// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Asset/DigumAssetManager.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(DigumAssetManager)

//////////////////////////////////////////////////////////////////////

static FAutoConsoleCommand CVarDumpLoadedAssets(
	TEXT("Digum.DumpLoadedAssets"),
	TEXT("Shows all assets that were loaded via the asset manager and are currently in memory."),
	FConsoleCommandDelegate::CreateStatic(UDigumAssetManager::DumpLoadedAssets)
);

//////////////////////////////////////////////////////////////////////


UDigumAssetManager::UDigumAssetManager()
{
}

UDigumAssetManager& UDigumAssetManager::Get()
{
	check(GEngine);
	if(UDigumAssetManager* Instance = Cast<UDigumAssetManager>(GEngine->AssetManager))
	{
		return *Instance;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to RPG AssetManager!"));

	return *NewObject<UDigumAssetManager>();
}


UObject* UDigumAssetManager::GetObjectWithPrimaryID(const FPrimaryAssetId& AssetId)
{
	FPrimaryAssetTypeInfo Info;
	if (Get().GetPrimaryAssetTypeInfo(AssetId.PrimaryAssetType, Info) && !Info.bHasBlueprintClasses)
	{
		return Get().GetPrimaryAssetObject(AssetId);
	}

	return nullptr;
}

void UDigumAssetManager::DumpLoadedAssets()
{
	UE_LOG(LogTemp, Log, TEXT("========== Start Dumping Loaded Assets =========="));

	for (const UObject* LoadedAsset : Get().LoadedAssets)
	{
		UE_LOG(LogTemp, Log, TEXT("  %s"), *GetNameSafe(LoadedAsset));
	}

	UE_LOG(LogTemp, Log, TEXT("... %d assets in loaded pool"), Get().LoadedAssets.Num());
	UE_LOG(LogTemp, Log, TEXT("========== Finish Dumping Loaded Assets =========="));
}

UObject* UDigumAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if(AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		// Check if Asset Logging is set
		if(ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// try load
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UDigumAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UDigumAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}