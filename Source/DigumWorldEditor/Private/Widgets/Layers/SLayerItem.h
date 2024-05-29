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
	SLATE_ARGUMENT(bool, bIsActive);
	SLATE_ATTRIBUTE(FDigumWorldAssetLayer*, Layer)
	SLATE_END_ARGS()

	virtual ~SLayerItem() override;

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
protected:
	TAttribute<int32> LayerIndexAttribute;
	TAttribute<FDigumWorldAssetLayer*> LayerAttribute;
	bool bIsActiveLayer = false;
	bool bVisible = true;
	void OnVisibilityChanged(ECheckBoxState NewState);

	DECLARE_MULTICAST_DELEGATE(FOnLayerUpdated);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetLayerName, const FText&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetLayerVisibility, const bool&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetLayerHierarchy, const int32&);

public:
	FOnLayerUpdated OnLayerUpdated;
	FOnSetLayerName OnSetLayerName;
	FOnSetLayerVisibility OnSetLayerVisibility;
	FOnSetLayerHierarchy OnSetLayerHierarchy;

	ECheckBoxState GetVisibilityState() const;
	TSharedPtr<SWidget> OnCreateVisibilityWidget();
	FText GetLayerName() const;
	void OnLayerNameCommitted(const FText& Text, ETextCommit::Type Arg);
	TSharedPtr<SWidget> OnCreateLayerNameWidget();
	void OnLayerHierarchyCommitted(const FText& Text, ETextCommit::Type Arg);
	void OnLayerHierarchyChanged(const FText& Text);
	FText GetLayerHierarchyText() const;
	TSharedPtr<SWidget> OnCreateLayerHierarchyWidget();

};
