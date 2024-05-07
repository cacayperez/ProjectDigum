// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DigumGameDeveloperSettings.generated.h"

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
	TSoftObjectPtr<UDigumInputSettingsAsset> PrimaryMinerInputSettings;
	
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Drag Inventory Slot Widget"))
	TSoftClassPtr<UDragDropOperation> DragDropOperationClass;
	
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Drag Inventory Slot Widget"))
	TSoftClassPtr<UUserWidget> DragDropWidgetClass;

	static TSubclassOf<UDragDropOperation> GetDragDropOperationClass();
	static TSubclassOf<UUserWidget> GetDragDropWidgetClass();
};
