// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UDigumUISettings.generated.h"

class UDigumWidgetStyle;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Digum UI Settings") )
class DIGUMUI_API UUDigumUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Game", meta=(DisplayName="Drag Inventory Slot Widget"))
	TSoftClassPtr<UDigumWidgetStyle> DefaultWindowStyle;

	static TSubclassOf<UDigumWidgetStyle> GetDefaultWidgetStyleClass();
	static UDigumWidgetStyle* GetDefaultWidgetStyle();
};
