// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Settings/DigumGameDeveloperSettings.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UDragDropOperation> UDigumGameDeveloperSettings::GetDragDropOperationClass()
{
	const UDigumGameDeveloperSettings* Settings = GetDefault<UDigumGameDeveloperSettings>();
	if(Settings)
	{
		return Settings->DragDropOperationClass.LoadSynchronous();
	}

	return nullptr;
}

TSubclassOf<UUserWidget> UDigumGameDeveloperSettings::GetDragDropWidgetClass()
{
	const UDigumGameDeveloperSettings* Settings = GetDefault<UDigumGameDeveloperSettings>();
	if(Settings)
	{
		return Settings->DragDropWidgetClass.LoadSynchronous();
	}

	return nullptr;
}
