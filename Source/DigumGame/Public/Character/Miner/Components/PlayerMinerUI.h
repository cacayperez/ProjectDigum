// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMinerUI.generated.h"

class SDigumWindow;
class SWidget;
class ADigumMinerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMGAME_API UPlayerMinerUI : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY()
	TWeakObjectPtr<ADigumMinerCharacter> OwningMiner;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningController;
	TSharedPtr<SOverlay> Overlay;
	TSharedPtr<SDigumWindow> InventorySlateWidget;
	TSharedPtr<SDigumWindow> CharacterMenuSlateWidget;
	
	TWeakPtr<SWidget> _Container;
	TWeakPtr<SWidget> InventoryContainer;
	TWeakPtr<SWidget> CharacterMenuContainer;


	
public:
	// Sets default values for this component's properties
	UPlayerMinerUI();

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
	virtual void InitializeInventoryUI();
	virtual void InitializeCharacterMenu();

	void PopLastWindow();
};
