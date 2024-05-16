#pragma once

#include "AssetTypeActions_Base.h"
#include "DigumWorldEditorToolkit.h"
#include "Asset/DigumWorldAsset.h"

class FDigumWorldEditor_AssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "DigumWorldAsset", "2D Grid Map Layout"); }
	virtual FColor GetTypeColor() const override { return FColor::Turquoise; }
	virtual UClass* GetSupportedClass() const override { return UDigumWorldAsset::StaticClass(); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override
	{
		for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			auto GridMapLayout = Cast<UDigumWorldAsset>(*ObjIt);
			if (GridMapLayout != nullptr)
			{
				TSharedRef<FDigumWorldEditorToolkit> EditorToolkit = MakeShared<FDigumWorldEditorToolkit>();
				EditorToolkit->Initialize(GridMapLayout, EToolkitMode::Standalone, EditWithinLevelEditor);
			}
		}
	}
};
