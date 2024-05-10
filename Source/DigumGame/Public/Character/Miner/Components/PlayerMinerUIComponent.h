// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMinerUIComponent.generated.h"

class UDigumWidget;
class UDigumInventoryWidget;
class SDigumWidget;
class SDigumWidgetStack;
class SDigumWindow;
class SDigumInventoryWindow;
class SWidget;
class ADigumMinerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UPlayerMinerUIComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<ADigumMinerCharacter> OwningMiner;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningController;
	
	TSharedPtr<SDigumWidgetStack> WidgetStack;

	TSharedPtr<SDigumWindow> CharacterMenuSlateWidget;
	TSharedPtr<SWidget> _Container;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDigumInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UDigumInventoryWidget> InventoryWidget;

	UPROPERTY()
	TObjectPtr<UDigumWidget> CharacterWidget;
	
	
public:
	// Sets default values for this component's properties
	UPlayerMinerUIComponent();

protected:
	UPROPERTY()
	bool bShowInventory = false;

	UPROPERTY()
	bool bShowCharacterMenu = false;
	
	UFUNCTION()
	virtual void OnToggleInventory();

	UFUNCTION()
	virtual void OnToggleCharacterMenu();

	UFUNCTION()
	virtual void OnCancelAction();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitializeUI();
	virtual void InitializeInventoryWidget();
	virtual void InitializeCharacterMenuWidget();

};
