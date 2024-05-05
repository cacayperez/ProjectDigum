// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/PlayerMinerUI.h"

#include "Character/DigumMinerCharacter.h"
#include "Widgets/SWeakWidget.h"
#include "Window/SDigumWindow.h"


// Sets default values for this component's properties
UPlayerMinerUI::UPlayerMinerUI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerMinerUI::OnToggleInventory()
{
	if(InventorySlateWidget.IsValid())
		InventorySlateWidget->ToggleVisibility();
}

void UPlayerMinerUI::OnToggleCharacterMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerMinerInventoryUI::OnToggleInventory"));
	
	if(CharacterMenuSlateWidget.IsValid())
		CharacterMenuSlateWidget->ToggleVisibility();
}

void UPlayerMinerUI::OnCancelAction()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerMinerInventoryUI::OnCancelAction"));
	PopLastWindow();
}


// Called when the game starts
void UPlayerMinerUI::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningMiner = Cast<ADigumMinerCharacter>(GetOwner());
	
	if(OwningMiner.IsValid())
	{
		OwningController = OwningMiner->GetLocalViewingPlayerController();
		
	}
 
	InitializeUI();
	
}

void UPlayerMinerUI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GEngine && GEngine->GameViewport)
	{
		// to do check for null lambda
		GEngine->GameViewport->RemoveViewportWidgetContent(_Container.Pin().ToSharedRef());
		
	}
}

void UPlayerMinerUI::InitializeUI()
{
	if(GEngine && GEngine->GameViewport)
	{
		Overlay = SNew(SOverlay);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(_Container, SWeakWidget).PossiblyNullContent(Overlay.ToSharedRef()));
		
		InitializeInventoryUI();
		InitializeCharacterMenu();
		if(OwningController.IsValid())
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventorySlateWidget);
			OwningController->SetInputMode(InputMode);
			OwningController->bShowMouseCursor = true;
		}
	}

	if(OwningMiner.IsValid())
	{
		OwningMiner->OnToggleInventoryDelegate().AddUObject(this, &UPlayerMinerUI::OnToggleInventory);
		OwningMiner->OnToggleCharacterMenuDelegate().AddUObject(this, &UPlayerMinerUI::OnToggleCharacterMenu);
	}
}

void UPlayerMinerUI::InitializeInventoryUI()
{
	// Initialize Inventory
	InventorySlateWidget = SNew(SDigumWindow);
	bShowInventory = false;
	InventorySlateWidget->SetVisibility(EVisibility::Hidden);

	if(_Container.IsValid())
	{
		Overlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			InventorySlateWidget.ToSharedRef()
		];
	}
}

void UPlayerMinerUI::InitializeCharacterMenu()
{
	CharacterMenuSlateWidget = SNew(SDigumWindow);
	bShowCharacterMenu = false;
	CharacterMenuSlateWidget->SetVisibility(EVisibility::Hidden);

	if(_Container.IsValid())
	{
		Overlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			CharacterMenuSlateWidget.ToSharedRef()
		];
	}
}

void UPlayerMinerUI::PopLastWindow()
{
	// TODO
	int32 Length = Overlay->GetChildren()->Num();
	
}



