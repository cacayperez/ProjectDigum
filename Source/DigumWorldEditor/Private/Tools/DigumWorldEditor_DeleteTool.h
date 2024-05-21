// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tools/DigumWorldEditorTool.h"
#include "UObject/Object.h"
#include "DigumWorldEditor_DeleteTool.generated.h"

/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditor_DeleteTool : public UDigumWorldEditorTool
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<SWidget> CreateToolWidget() override;
	virtual void OnActivateTool(const FDigumWorldEditorToolParams& InParams) override;
	virtual FText GetToolName() const override;
};
