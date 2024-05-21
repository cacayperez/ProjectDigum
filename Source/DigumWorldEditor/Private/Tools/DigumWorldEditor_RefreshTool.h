// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tools/DigumWorldEditorTool.h"
#include "UObject/Object.h"
#include "DigumWorldEditor_RefreshTool.generated.h"

class UDigumWorldAsset;
/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditor_RefreshTool : public UDigumWorldEditorTool
{
	GENERATED_BODY()
	
public:
	virtual void OnActivateTool(const FDigumWorldEditorToolParams& InParams) override;
	virtual FText GetToolName() const override;
	virtual TSharedPtr<SWidget> CreateToolWidget() override;

protected:
	void CleanUpSwatches(UDigumWorldAsset* Asset);
	void CleanUpLayers(UDigumWorldAsset* Asset);
	void CleanUpCoordinates(UDigumWorldAsset* Asset);
};


