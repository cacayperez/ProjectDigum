#pragma once

#include "Asset/DigumWorldAsset.h"
#include "Toolkits/AssetEditorToolkit.h"

class UDigumWorldEditorTool;
class UDigumWorldAsset;
class UDigumWorldEditorSwatch;

class FDigumWorldEditorToolkit : public FAssetEditorToolkit
{
public:
	~FDigumWorldEditorToolkit();
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override { return FName("DigumWorldEditor"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("DigumWorldEditor", "DigumWorldEditor", "Digum World Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return NSLOCTEXT("DigumWorldEditor", "DigumWorldEditorTabPrefix", "DigumWorldEditor").ToString(); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Yellow; }
	void OnLayerUpdated();

protected:
	TSharedRef<SDockTab> SpawnTab_Tools(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Layers(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Swatches(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_CanvasViewport(const FSpawnTabArgs& SpawnTabArgs);
	
	TObjectPtr<UDigumWorldAsset> AssetBeingEdited;
	TArray<TObjectPtr<UDigumWorldEditorTool>> Tools;
 
	int32 ActiveLayerIndex = 0;
	int32 ActiveSwatchIndex = 0;
	int32 ActiveToolIndex = 0;
	
	void InitializeTools();
	UDigumWorldEditorTool* GetActiveTool() const;
public:
	void Initialize(UDigumWorldAsset* InWorldAssetBeingEdited, EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InInitToolkitHost);
	
	UDigumWorldAsset* GetAssetBeingEdited() const { return AssetBeingEdited; };
	void AddSwatchItem(UDigumWorldEditorSwatch* InSwatch);
	void AddNewLayer();
	void DeleteLayer(const int32& InIndex);
	
	void UpdateLayer(const int32& InLayerIndex, const FDigumWorldAssetLayer& InLayer);
	void DeleteActiveLayer();
	void SetActiveLayerIndex(const int32 InLayerIndex);
	void SetActiveSwatchIndex(const int32 InSwatchIndex);
	void PlaceCoordinate(const int32& InX, const int32& InY);
	int32 GetActiveLayerIndex() const;
	int32 GetActiveSwatchIndex() const;
	void AddCoordinate(const int32& InLayerIndex, const int32& InX, const int32& InY);
	void AddCoordinateToActiveLayer(const int32& InX, const int32& InY);
	void CallToolAction(const int32& InLayerIndex, const int32& InX, const int32& InY);
	void CallToolAction(const int32& InX, const int32& InY);
	void SetLayerName(const int32& InLayerIndex, const FText& InLayerName);
	void SetLayerVisibility(const int32& InLayerIndex, const bool& bInLayerVisibility);
	void SetActiveTool(const int32& InToolIndex);

	TArray<UDigumWorldEditorTool*> GetTools();
	int32 GetActiveToolIndex() const { return ActiveToolIndex;};
	void SwapLayers(const int32 InLayerIndexA, const int32 InLayerIndexB);
};
