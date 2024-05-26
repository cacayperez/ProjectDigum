// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
#include "UObject/Object.h"
#include "DigumWorldEditorTool.generated.h"

struct FDigumWorldAssetCoordinateArray;
struct FDigumWorldAssetCoordinate;
class UDigumWorldAsset;

USTRUCT()
struct FDigumWorldEditorToolParams
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UDigumWorldAsset* Asset = nullptr;

	UPROPERTY()
	int32 LayerIndex = INDEX_NONE;

	UPROPERTY()
	int32 SwatchINdex = INDEX_NONE;

	UPROPERTY()
	FDigumWorldAssetCoordinateArray Selection;
	
	/*
	UPROPERTY()
	int32 X = INDEX_NONE;

	UPROPERTY()
	int32 Y = INDEX_NONE;*/
};
/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditorTool : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText ToolName;

	UPROPERTY()
	bool bIsActive;
	
protected:
	DECLARE_MULTICAST_DELEGATE(FOnSelectToolDelegate);
	
	virtual void OnActivateTool(const FDigumWorldEditorToolParams& InParams);
public:
	FOnSelectToolDelegate OnSelectToolDelegate;
	
	virtual void ActivateTool(const FDigumWorldEditorToolParams& InParams);
	virtual TSharedPtr<SWidget> CreateToolWidget();
	virtual FText GetToolName() const;
	virtual void SetActive(const bool& bInActive);
	virtual bool IsActive() const;
};
