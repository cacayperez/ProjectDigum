#pragma once

#include "Toolkits/AssetEditorToolkit.h"

class UDigumWorldAsset;
class UDigumWorldEditorSwatch;

class FDigumWorldEditorToolkit : public FAssetEditorToolkit
{
public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override { return FName("DigumWorldEditor"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("DigumWorldEditor", "DigumWorldEditor", "Digum World Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return NSLOCTEXT("DigumWorldEditor", "DigumWorldEditorTabPrefix", "DigumWorldEditor").ToString(); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Yellow; }

protected:
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Layers(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Swatches(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_CanvasViewport(const FSpawnTabArgs& SpawnTabArgs);
	
	TObjectPtr<UDigumWorldAsset> AssetBeingEdited;

	int32 ActiveLayerIndex = 0;
	int32 ActiveSwatchIndex = 0;

public:
	void Initialize(UDigumWorldAsset* InWorldAssetBeingEdited, EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InInitToolkitHost);
	
	UDigumWorldAsset* GetAssetBeingEdited() const { return AssetBeingEdited; };
	void AddSwatchItem(UDigumWorldEditorSwatch* InSwatch);
	void AddNewLayer();
	void DeleteLayer(const int32& InIndex);

	void SetActiveLayerIndex(const int32 InLayerIndex);
	void SetActiveSwatchIndex(const int32 InSwatchIndex);
	void PlaceCoordinate(const int32& InX, const int32& InY);
	int32 GetActiveLayerIndex() const;
	int32 GetActiveSwatchIndex() const;
};
