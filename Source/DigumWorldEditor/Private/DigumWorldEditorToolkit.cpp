#include "DigumWorldEditorToolkit.h"

#include "SSingleObjectDetailsPanel.h"
#include "Asset/DigumWorldAsset.h"
#include "Objects/DigumWorldEditorSwatch.h"
#include "Tools/DigumWorldEditor_AddTool.h"
#include "Tools/DigumWorldEditor_DeleteTool.h"
#include "Tools/DigumWorldEditor_RefreshTool.h"
#include "Widgets/Canvas/SCanvasTab.h"
#include "Widgets/Layers/SLayerTab.h"
#include "Widgets/Swatch/SSwatchTab.h"
#include "Widgets/Tools/SToolTab.h"

struct FDigumWorldEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName CanvasViewportID;
	static const FName ToolsID;
	// static const FName ActionsSubMenuID;
	static const FName SwatchesID;
	static const FName LayersID;
	
};

const FName FDigumWorldEditorTabs::DetailsID(TEXT("DigumGridMapLayoutEditor_Details"));
const FName FDigumWorldEditorTabs::CanvasViewportID(TEXT("DigumGridMapLayoutEditor_CanvasViewport"));
const FName FDigumWorldEditorTabs::ToolsID(TEXT("DigumGridMapLayoutEditor_Tools"));
// const FName FDigumWorldEditorTabs::ActionsSubMenuID(TEXT("DigumGridMapLayoutEditor_ActionsSubMenu"));
const FName FDigumWorldEditorTabs::SwatchesID(TEXT("DigumGridMapLayoutEditor_Swatches"));
const FName FDigumWorldEditorTabs::LayersID(TEXT("DigumGridMapLayoutEditor_Layers"));

class SDigumWorldEditorDetailsTab : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SDigumWorldEditorDetailsTab) {}
	SLATE_END_ARGS()

private:
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;

public:
	void Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
	{
		ToolkitPtr = InToolkit;
		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InToolkit->GetToolkitCommands()).HostTabManager(InToolkit->GetTabManager()), true, true);
	}

	virtual UObject* GetObjectToObserve() const override
	{
		return ToolkitPtr.Pin()->GetAssetBeingEdited();
	}

	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override
	{
		return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			PropertyEditorWidget
		];
	}
};

void FDigumWorldEditorToolkit::OnLayerUpdated()
{
	
}


TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_Details(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	
	return SNew(SDockTab)
		.Label(NSLOCTEXT("DetailsTab_Title", "DetailsTab", "Details"))
		[
			//SNew(SBorder)
			SNew(SDigumWorldEditorDetailsTab, WorldEditorToolkit)
		];	
}

TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_Layers(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	
	return SNew(SDockTab)
		.Label(NSLOCTEXT("LayersTab_Title", "LayersTab", "Layers"))
		[
			//SNew(SBorder)
			SNew(SLayerTab, WorldEditorToolkit)
		];	
}

TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_Swatches(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	
	return SNew(SDockTab)
	.Label(NSLOCTEXT("SwatchesTab_Title", "SwatchesTab", "Swatches"))
	[
		//SNew(SBorder)
		SNew(SSwatchTab, WorldEditorToolkit)
	];	
}

TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_CanvasViewport(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	return SNew(SDockTab)
	.Label(NSLOCTEXT("CanvasTab_Title", "CanvasTab", "Canvas"))
	[
		//SNew(SBorder)
		SNew(SCanvasTab, WorldEditorToolkit)
	];
}


TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_Tools(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	return SNew(SDockTab)
	.Label(NSLOCTEXT("ToolsTab_Title", "ToolsTab", "Tools"))
	[
		//SNew(SBorder)
		SNew(SToolTab, WorldEditorToolkit)
	];
}

FDigumWorldEditorToolkit::~FDigumWorldEditorToolkit()
{
	PaintTools.Empty();
	UtilityTools.Empty();
}

void FDigumWorldEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(NSLOCTEXT("DigumWorldEditor", "WorkspaceMenu_DigumWorldEditor", "2D Grid Map Layout Editor"));
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FDigumWorldEditorTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FDigumWorldEditorToolkit::SpawnTab_Details))
	.SetDisplayName(NSLOCTEXT("DigumWorldEditor", "DetailsTab", "Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(FDigumWorldEditorTabs::LayersID, FOnSpawnTab::CreateSP(this, &FDigumWorldEditorToolkit::SpawnTab_Layers))
	.SetDisplayName(NSLOCTEXT("DigumWorldEditor", "LayersTab", "Layers"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(FDigumWorldEditorTabs::SwatchesID, FOnSpawnTab::CreateSP(this, &FDigumWorldEditorToolkit::SpawnTab_Swatches))
	.SetDisplayName(NSLOCTEXT("DigumWorldEditor", "SwatchesTab", "Swatches"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(FDigumWorldEditorTabs::CanvasViewportID, FOnSpawnTab::CreateSP(this, &FDigumWorldEditorToolkit::SpawnTab_CanvasViewport))
	.SetDisplayName(NSLOCTEXT("DigumWorldEditor", "CanvasTab", "Canvas"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(FDigumWorldEditorTabs::ToolsID, FOnSpawnTab::CreateSP(this, &FDigumWorldEditorToolkit::SpawnTab_Tools))
	.SetDisplayName(NSLOCTEXT("DigumWorldEditor", "ToolsTab", "Tools"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FDigumWorldEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

void FDigumWorldEditorToolkit::InitializeTools()
{
	PaintTools.Add(NewObject<UDigumWorldEditor_AddTool>());
	PaintTools.Add(NewObject<UDigumWorldEditor_DeleteTool>());
	UtilityTools.Add(NewObject<UDigumWorldEditor_RefreshTool>());
}

UDigumWorldEditorTool* FDigumWorldEditorToolkit::GetActivePaintTool() const
{
	if(PaintTools.IsValidIndex(ActivePaintToolIndex))
	{
		return PaintTools[ActivePaintToolIndex];
	}

	return nullptr;
}

void FDigumWorldEditorToolkit::Initialize(UDigumWorldAsset* InWorldAssetBeingEdited, EToolkitMode::Type InMode,
                                          const TSharedPtr<IToolkitHost>& InInitToolkitHost)
{
	InitializeTools();
	AssetBeingEdited = InWorldAssetBeingEdited;
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_DigumWorldEditor_Layout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
					
			FTabManager::NewStack()
				->AddTab(FDigumWorldEditorTabs::LayersID, ETabState::OpenedTab)
				->SetHideTabWell(true)
		)
	);
		
	InitAssetEditor(
		InMode,
		InInitToolkitHost,
		"DigumGridMapLayoutEditor",
		StandaloneDefaultLayout,
		true,
		true,
		AssetBeingEdited
		);

}

void FDigumWorldEditorToolkit::AddSwatchItem(UDigumWorldEditorSwatch* InSwatch)
{
	if(!InSwatch->IsValidSwatch()) return;

	if(GEditor)
	{
		FDigumWorldSwatchPaletteItem NewSwatchItem = FDigumWorldSwatchPaletteItem();
		NewSwatchItem.SwatchName = InSwatch->SwatchName;
		NewSwatchItem.SoftSwatchAsset = InSwatch->SoftSwatchAsset;
		
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: Add Swatch"));
		GetAssetBeingEdited()->Modify();
		GetAssetBeingEdited()->AddSwatchPaletteItem(NewSwatchItem);
		GEditor->EndTransaction();
	}
}

void FDigumWorldEditorToolkit::AddNewLayer()
{
	if(GEditor)
	{
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: Add Layer"));
		GetAssetBeingEdited()->Modify();
		AssetBeingEdited->AddNewLayer();
		GEditor->EndTransaction();
	}
}

void FDigumWorldEditorToolkit::DeleteLayer(const int32& InIndex)
{
	if(GEditor && GetAssetBeingEdited())
	{
		int32 TempIndex = ActiveLayerIndex;
		const int32 LastIndex = GetAssetBeingEdited()->GetLayers().Num() -1;
		if(ActiveLayerIndex == LastIndex)
		{
			TempIndex = FMath::Clamp<int32>(ActiveLayerIndex-1, 0, LastIndex - 1);
		}
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: DeleteLayer"));
		GetAssetBeingEdited()->Modify();
		GetAssetBeingEdited()->DeleteLayer(InIndex);
		GEditor->EndTransaction();

		ActiveLayerIndex = TempIndex;
	}
}

void FDigumWorldEditorToolkit::UpdateLayer(const int32& InLayerIndex, const FDigumWorldAssetLayer& InLayer)
{
	FDigumWorldAssetLayer* Layer = GetAssetBeingEdited()->GetLayer(InLayerIndex);
	if(GEditor && GetAssetBeingEdited())
	{
		GetAssetBeingEdited()->UpdateLayer(InLayerIndex, InLayer);
	}
}

void FDigumWorldEditorToolkit::DeleteActiveLayer()
{
	DeleteLayer(ActiveLayerIndex);
}

void FDigumWorldEditorToolkit::SetActiveLayerIndex(const int32 InLayerIndex)
{
	if(InLayerIndex >= 0 && InLayerIndex < GetAssetBeingEdited()->GetLayers().Num())
	{
		ActiveLayerIndex = InLayerIndex;
		return;
	}
	ActiveLayerIndex = 0;
}

void FDigumWorldEditorToolkit::SetActiveSwatchIndex(const int32 InSwatchIndex)
{
	ActiveSwatchIndex = InSwatchIndex;
}

void FDigumWorldEditorToolkit::PlaceCoordinate(const int32& InX, const int32& InY)
{
}

int32 FDigumWorldEditorToolkit::GetActiveLayerIndex() const
{
	return ActiveLayerIndex;
}

int32 FDigumWorldEditorToolkit::GetActiveSwatchIndex() const
{
	return ActiveSwatchIndex;
}

void FDigumWorldEditorToolkit::AddCoordinate(const int32& LayerIndex, const int32& X, const int32& Y)
{
	if(GEditor && GetAssetBeingEdited())
	{
		FDigumWorldSwatchPaletteItem* Swatch = GetAssetBeingEdited()->GetSwatch(ActiveSwatchIndex);
		FDigumWorldAssetLayer* Layer = GetAssetBeingEdited()->GetLayer(ActiveLayerIndex);
		if(Swatch && Layer)
		{
			FName SwatchName = Swatch->SwatchName;
			
			FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(X, Y, SwatchName);
			GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: AddCoordinate"));
			GetAssetBeingEdited()->Modify();
			Layer->AddCoordinate(Coordinate);
			GEditor->EndTransaction();
		}
	}
}

void FDigumWorldEditorToolkit::AddCoordinateToActiveLayer(const int32& InX, const int32& InY)
{
	AddCoordinate(ActiveLayerIndex, InX, InY);
}

void FDigumWorldEditorToolkit::CallToolAction(const int32& InLayerIndex, const int32& InX, const int32& InY)
{
	if(GetActivePaintTool())
	{
		FDigumWorldEditorToolParams Params;
		Params.Asset = GetAssetBeingEdited();
		Params.LayerIndex = InLayerIndex;
		Params.SwatchINdex = ActiveSwatchIndex;
		Params.X = InX;
		Params.Y = InY;
		GetActivePaintTool()->ActivateTool(Params);	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Active Tool"));
	}
}

void FDigumWorldEditorToolkit::CallToolAction(const int32& InX, const int32& InY)
{
	CallToolAction(ActiveLayerIndex, InX, InY);
}

void FDigumWorldEditorToolkit::CallToolAction(const int32& InUtilityToolIndex)
{
	if(UtilityTools.IsValidIndex(InUtilityToolIndex))
	{
		FDigumWorldEditorToolParams Params;
		Params.Asset = GetAssetBeingEdited();

		UtilityTools[InUtilityToolIndex]->ActivateTool(Params);
	}
}


void FDigumWorldEditorToolkit::SetLayerName(const int32& InLayerIndex, const FText& InLayerName)
{
	if(GEditor && GetAssetBeingEdited())
	{
		FText Name = InLayerName;
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: SetLayerName"));
		GetAssetBeingEdited()->Modify();
		GetAssetBeingEdited()->SetLayerName(InLayerIndex, Name);
		GEditor->EndTransaction();
	}
}

void FDigumWorldEditorToolkit::SetLayerVisibility(const int32& InLayerIndex, const bool& bInLayerVisibility)
{
	if(GEditor && GetAssetBeingEdited())
	{
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: SetLayerVisibility"));
		GetAssetBeingEdited()->Modify();
		GetAssetBeingEdited()->SetLayerVisibility(InLayerIndex, bInLayerVisibility);
		GEditor->EndTransaction();
		
	}
}

void FDigumWorldEditorToolkit::SetActiveTool(const int32& InToolIndex)
{
	ActivePaintToolIndex = InToolIndex;
}

TArray<UDigumWorldEditorTool*> FDigumWorldEditorToolkit::GetPaintTools()
{
	return PaintTools;
}

TArray<UDigumWorldEditorTool*> FDigumWorldEditorToolkit::GetUtilityTools()
{
	return UtilityTools;
}

void FDigumWorldEditorToolkit::SwapLayers(const int32 InLayerIndexA, const int32 InLayerIndexB)
{
	if(GEditor && GetAssetBeingEdited())
	{
		int32 OutEndIndex;
		GEditor->BeginTransaction(FText::FromString("DigumWorldEditor: MoveLayer"));
		GetAssetBeingEdited()->Modify();
		GetAssetBeingEdited()->SwapLayers(InLayerIndexA, InLayerIndexB, OutEndIndex);

		// TODO - Figure out how to revert active layer index on undo.
		SetActiveLayerIndex(OutEndIndex);
		GEditor->EndTransaction();
	}
}
