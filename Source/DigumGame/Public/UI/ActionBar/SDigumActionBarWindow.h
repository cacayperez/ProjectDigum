// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SDigumInventoryWindow.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMGAME_API SDigumActionBarWindow : public SDigumInventoryWindow
{
protected:
  TSharedPtr<SWidget> ActionBarWindow;
  
 // virtual TSharedPtr<SWidget> OnCreateWindow() override;
  //virtual void DrawWindow() override;
  /*virtual TSharedPtr<SWidget> OnCreateBackground() override;*/
  virtual void OnReceiveDropPayload(UObject* InPayload) override;

};
