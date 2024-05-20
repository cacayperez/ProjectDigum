// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/Base/SWidgetBase.h"

struct FDigumWorldAssetLayer;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SLayerItem : public SWidgetBase
{
public:
	SLATE_BEGIN_ARGS(SLayerItem)
		{
		}
	SLATE_ATTRIBUTE(int32, LayerIndex)
	SLATE_ATTRIBUTE(FDigumWorldAssetLayer, Layer)
	SLATE_END_ARGS()

	void SelectLayer();
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
protected:
	TAttribute<int32> LayerIndexAttribute;
	TAttribute<FDigumWorldAssetLayer> LayerAttribute;
	bool bSelected = false;
	bool bVisible = true;
	void OnVisibilityChanged(ECheckBoxState NewState);

	DECLARE_DELEGATE_TwoParams(FOnLayerUpdated, const int32&, const FDigumWorldAssetLayer&);
	DECLARE_DELEGATE_OneParam(FOnLayerSelected, const int32&)

public:
	FOnLayerUpdated OnLayerUpdated;
	FOnLayerSelected OnLayerSelected;
	void SetSelected(const bool bValue) { bSelected = bValue; }

	ECheckBoxState GetVisibilityState() const;
	TSharedPtr<SWidget> OnCreateVisibilityWidget();
	FText GetLayerName() const;
	void OnLayerNameCommited(const FText& Text, ETextCommit::Type Arg);
	TSharedPtr<SWidget> OnCreateLayerNameWidget();	
};
