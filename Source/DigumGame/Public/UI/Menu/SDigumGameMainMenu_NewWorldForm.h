// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumGameMainMenu_NewWorldForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumGameMainMenu_NewWorldForm)
		{
		}

	SLATE_END_ARGS()


	
	FText GetWorldNameText() const;
	void OnWorldNameChanged(const FText& Text);
	void OnWorldNameCommitted(const FText& Text, ETextCommit::Type Arg);
	FText GetWorldSeedText() const;
	void OnWorldSeedChanged(const FText& Text);
	void OnWorldSeedCommitted(const FText& Text, ETextCommit::Type Arg);
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	int32 CurrentSeed = 0;
	FText WorldName;
	FText WorldSeed;
	
};
