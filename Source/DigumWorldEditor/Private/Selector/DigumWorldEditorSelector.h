// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWorldEditorSelector.generated.h"

struct FDigumWorldAssetCoordinate;
/**
 * 
 */
UCLASS()
class DIGUMWORLDEDITOR_API UDigumWorldEditorSelector : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bHeldDown = false;
	
	UPROPERTY()
	TArray<FDigumWorldAssetCoordinate> Selection;
	
public:
	virtual TSharedPtr<SWidget> CreateSelectorWidget();
	void AddSelection(FDigumWorldAssetCoordinate Coordinate);
	void ClearSelection();
	void EndSelection();
};
