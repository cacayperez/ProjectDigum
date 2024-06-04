// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DigumGameDeveloperSettings.generated.h"

class UDigumWindow;
struct FDigumContentCategory;
class UDigumWindowStyle;
class UDragDropOperation;
class UDigumInputSettingsAsset;
class UUserWidget;
class UDigumGameMainMenuWindow;

USTRUCT()
struct FDigumGameLevelSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Game | Level Settings", meta=(DisplayName="Main Menu Widget"))
	TSoftClassPtr<UDigumGameMainMenuWindow> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Game | Level Settings", meta=(DisplayName="Main Menu Level"))
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, Category = "Game | Level Settings", meta=(DisplayName="Game Level"))
	TSoftObjectPtr<UWorld> GameMenuLevel;

	UPROPERTY(EditAnywhere, Category = "Game | Level Settings", meta=(DisplayName="Loading Level"))
	TSoftObjectPtr<UWorld> LoadingTransitionLevel;
};


/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Digum Game Settings") )
class DIGUMGAME_API UDigumGameDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Game Content"))
	TArray<FDigumContentCategory> ContentCategories;
	
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Primary Miner Input"))
	TSoftObjectPtr<UDigumInputSettingsAsset> PrimaryMinerInputSettings;

	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Main Menu Widget"))
	FDigumGameLevelSettings GameLevelSettings;
	
	static const FDigumContentCategory* GetContentCategoryData(FName ContentCategoryName);
	static const UDigumGameDeveloperSettings* Get();
};
