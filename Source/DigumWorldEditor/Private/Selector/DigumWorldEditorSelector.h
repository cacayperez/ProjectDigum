// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/DigumWorldAsset.h"
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
	
protected:
	UPROPERTY()
	FDigumWorldAssetCoordinateArray Selection;

	UPROPERTY()
	bool bIsActive = false;
	DECLARE_MULTICAST_DELEGATE(FOnSelectSelector);
	DECLARE_MULTICAST_DELEGATE(FOnBeginSelection);
	DECLARE_MULTICAST_DELEGATE(FOnEndSelection);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetSelection, FDigumWorldAssetCoordinate);
	
	FOnBeginSelection OnBeginSelectionDelegate;
	FOnSetSelection OnSetSelectionDelegate;
	FOnEndSelection OnEndSelectionDelegate;
	FOnSelectSelector OnSelectSelectorDelegate;
public:
	virtual TSharedPtr<SWidget> CreateSelectorButtonWidget();
	virtual TSharedPtr<SWidget> CreateSelectorSelectionWidget();
	virtual void AddSelection(FDigumWorldAssetCoordinate Coordinate);
	virtual void ClearSelection();
	virtual void EndSelection();
	virtual void BeginSelection();

	FDigumWorldAssetCoordinateArray GetSelection() const { return Selection; }
	FOnBeginSelection& GetOnBeginSelection() { return OnBeginSelectionDelegate; }
	FOnSetSelection& GetOnSetSelection() { return OnSetSelectionDelegate; }
	FOnEndSelection& GetOnEndSelection() { return OnEndSelectionDelegate; }
	FOnSelectSelector& GetOnSelectSelector() { return OnSelectSelectorDelegate; }

	void SetActive(bool bInIsActive) { bIsActive = bInIsActive; }
	bool IsActive() const { return bIsActive; }
};
