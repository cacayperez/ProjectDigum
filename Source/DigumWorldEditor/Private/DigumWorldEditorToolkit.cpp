#include "DigumWorldEditorToolkit.h"

#include "SSingleObjectDetailsPanel.h"
#include "Asset/DigumWorldAsset.h"
#include "Objects/DigumWorldEditorSwatch.h"
#include "Widgets/Layers/SLayerTab.h"
#include "Widgets/Swatch/SSwatchTab.h"

struct FDigumWorldEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName CanvasViewportID;
	static const FName ActionsPaletteID;
	static const FName ActionsSubMenuID;
	static const FName SwatchesID;
	static const FName LayersID;
	
};

const FName FDigumWorldEditorTabs::DetailsID(TEXT("DigumGridMapLayoutEditor_Details"));
const FName FDigumWorldEditorTabs::CanvasViewportID(TEXT("DigumGridMapLayoutEditor_CanvasViewport"));
const FName FDigumWorldEditorTabs::ActionsPaletteID(TEXT("DigumGridMapLayoutEditor_ActionsPalette"));
const FName FDigumWorldEditorTabs::ActionsSubMenuID(TEXT("DigumGridMapLayoutEditor_ActionsSubMenu"));
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
		.Label(NSLOCTEXT("DetailsTab_Title", "LayersTab", "Layers"))
		[
			//SNew(SBorder)
			SNew(SLayerTab)
			.AssetBeingEdited(WorldAssetBeingEdited)
		];	
}

TSharedRef<SDockTab> FDigumWorldEditorToolkit::SpawnTab_Swatches(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<FDigumWorldEditorToolkit> WorldEditorToolkit = SharedThis(this);
	
	return SNew(SDockTab)
	.Label(NSLOCTEXT("DetailsTab_Title", "SwatchesTab", "Swatches"))
	[
		//SNew(SBorder)
		SNew(SSwatchTab, WorldEditorToolkit)
		.AssetBeingEdited(WorldAssetBeingEdited)
	];	
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
}

void FDigumWorldEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

void FDigumWorldEditorToolkit::Initialize(UDigumWorldAsset* InWorldAssetBeingEdited, EToolkitMode::Type InMode,
	const TSharedPtr<IToolkitHost>& InInitToolkitHost)
{
	WorldAssetBeingEdited = InWorldAssetBeingEdited;
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_DigumWorldEditor_Layout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
					
			FTabManager::NewStack()
				->AddTab(FDigumWorldEditorTabs::ActionsSubMenuID, ETabState::OpenedTab)
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
		WorldAssetBeingEdited
		);
}

void FDigumWorldEditorToolkit::AddSwatchItem(UDigumWorldEditorSwatch* InSwatch)
{
	// GetAssetBeingEdited()
	FDigumWorldSwatchPaletteItem NewSwatchItem = FDigumWorldSwatchPaletteItem();
	NewSwatchItem.SwatchName = InSwatch->SwatchName;
	NewSwatchItem.SoftSwatchAsset = InSwatch->SoftSwatchAsset;

	GetAssetBeingEdited()->AddSwatchPaletteItem(NewSwatchItem);
}
