// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/DigumGameMenuPlayerController.h"

#include "Asset/DigumAssetManager.h"
#include "Core/SDigumWidgetStack.h"
#include "Settings/DigumGameDeveloperSettings.h"
#include "UI/Menu/DigumGameMainMenuWindow.h"
#include "Widgets/SWeakWidget.h"

void ADigumGameMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(GEngine && GEngine->GameViewport)
	{
		WidgetStack = SNew(SDigumWidgetStack);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetStack);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(WidgetStack.ToSharedRef()));

		// InitializeUI();
	}
	
}

void ADigumGameMenuPlayerController::InitializeUI()
{
	if(const UDigumGameDeveloperSettings* Settings = GetDefault<UDigumGameDeveloperSettings>())
	{
		if(TSubclassOf<UDigumWindow> MainMenuWindowClass = UDigumAssetManager::GetSubclass<UDigumWindow>(Settings->GameLevelSettings.MainMenuWidgetClass))
		{
			// InventoryWidget = UDigumWidget::Create<UDigumInventoryWidget>(this, InventoryWidgetClass);
			MenuWindow = UDigumWidget::Create<UDigumWindow>(this, MainMenuWindowClass);
			if(WidgetStack)
			{
				WidgetStack->AddItemToStack(MenuWindow, HAlign_Fill, VAlign_Fill);
			}
		}
	}
}
