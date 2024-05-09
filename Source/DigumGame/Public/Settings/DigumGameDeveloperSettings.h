// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DigumGameDeveloperSettings.generated.h"

struct FDigumContentCategory;
class UDigumWindowStyle;
class UDragDropOperation;
class UDigumInputSettingsAsset;
class UUserWidget;



/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Digum Game Settings") )
class DIGUMGAME_API UDigumGameDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Primary Miner Input"))
	TArray<FDigumContentCategory> ContentCategories;
	
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Primary Miner Input"))
	TSoftObjectPtr<UDigumInputSettingsAsset> PrimaryMinerInputSettings;
	
	static const FDigumContentCategory* GetContentCategoryData(FName ContentCategoryName);
	static const UDigumGameDeveloperSettings* Get();
};
