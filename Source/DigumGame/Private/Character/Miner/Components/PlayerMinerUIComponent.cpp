// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/PlayerMinerUIComponent.h"

#include "Character/Miner/DigumMinerCharacter.h"
#include "Slate/SDigumInventoryWindow.h"
#include "Widgets/SWeakWidget.h"
#include "Window/SDigumWindow.h"


// Sets default values for this component's properties
UPlayerMinerUIComponent::UPlayerMinerUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerMinerUIComponent::OnToggleInventory()
{
	if(InventorySlateWidget.IsValid())
		InventorySlateWidget->ToggleVisibility();
}

void UPlayerMinerUIComponent::OnToggleCharacterMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerMinerInventoryUI::OnToggleInventory"));
	
	if(CharacterMenuSlateWidget.IsValid())
		CharacterMenuSlateWidget->ToggleVisibility();
}

void UPlayerMinerUIComponent::OnCancelAction()
{
	PopLastWindow();
}

// Called when the game starts
void UPlayerMinerUIComponent::BeginPlay()
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

void UPlayerMinerUIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GEngine && GEngine->GameViewport && _Container.IsValid())
	{
		
		GEngine->GameViewport->RemoveViewportWidgetContent(_Container.Pin().ToSharedRef());
	}

	// Clean up
	_Container.Reset();
	Overlay.Reset();
	InventorySlateWidget.Reset();
	CharacterMenuSlateWidget.Reset();
}

void UPlayerMinerUIComponent::InitializeUI()
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
		OwningMiner->OnToggleInventoryDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleInventory);
		OwningMiner->OnToggleCharacterMenuDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleCharacterMenu);
	}
}

void UPlayerMinerUIComponent::InitializeInventoryUI()
{
	// Initialize Inventory
	InventorySlateWidget = SNew(SDigumInventoryWindow).InventoryComponent(OwningMiner->GetInventoryComponent());
	//InventorySlateWidget->SetInventoryComponent(OwningMiner->GetInventoryComponent());
	
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

void UPlayerMinerUIComponent::InitializeCharacterMenu()
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

void UPlayerMinerUIComponent::PopLastWindow()
{
	// TODO
	int32 Length = Overlay->GetChildren()->Num();
	
}



