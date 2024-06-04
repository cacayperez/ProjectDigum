// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DigumGameMenuPlayerController.generated.h"

class UDigumWindow;
class SDigumWidgetStack;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API ADigumGameMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UDigumWindow> MenuWindow;
public:
	virtual void BeginPlay() override;

protected:
	TSharedPtr<SDigumWidgetStack> WidgetStack;

	virtual void InitializeUI();
};
