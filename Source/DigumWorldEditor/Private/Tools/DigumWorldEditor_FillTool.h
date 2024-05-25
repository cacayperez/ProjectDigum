// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tools/DigumWorldEditorTool.h"
#include "UObject/Object.h"
#include "DigumWorldEditor_FillTool.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditor_FillTool : public UDigumWorldEditorTool
{
	GENERATED_BODY()
public:
	virtual void OnActivateTool(const FDigumWorldEditorToolParams& InParams) override;
	virtual FText GetToolName() const override;
	virtual TSharedPtr<SWidget> CreateToolWidget() override;
};
