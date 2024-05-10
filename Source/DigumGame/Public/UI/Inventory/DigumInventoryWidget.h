// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object/DigumWidget.h"
#include "Object/DigumWindow.h"
#include "UObject/Object.h"
#include "DigumInventoryWidget.generated.h"

class UDigumGameInventoryComponent;
class SDigumInventoryWindow;
class UDigumInventoryComponent;
/**
 * 
 */
UCLASS()
class DIGUMGAME_API UDigumInventoryWidget : public UDigumWindow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Digum Window")
	TObjectPtr<UMaterialInterface> InventorySlotMaterial;
	
	TSharedPtr<SDigumInventoryWindow> InventorySlate;
	
public:
	~UDigumInventoryWidget();
	virtual void OnCreateWidget() override;
	virtual TSharedPtr<SDigumWidget> GetWidget() const override;
	virtual void SetInventoryComponent(UDigumGameInventoryComponent* InventoryComponent);
};
