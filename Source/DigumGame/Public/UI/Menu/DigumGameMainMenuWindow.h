// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object/DigumWindow.h"
#include "DigumGameMainMenuWindow.generated.h"

class SDigumGameMainMenuWindow;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumGameMainMenuWindow : public UDigumWindow
{
	GENERATED_BODY()

	TSharedPtr<SDigumGameMainMenuWindow> MainMenuSlate;
	
public:
	virtual void OnCreateWidget() override;
	virtual TSharedPtr<SDigumWidget> GetWidget() const override;
};
