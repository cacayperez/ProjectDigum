// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/PlayerMinerUIComponent.h"

#include "Character/Miner/DigumMinerCharacter.h"
#include "Core/SDigumWidgetStack.h"
#include "UI/Inventory/SDigumInventoryWindow.h"
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
	WidgetStack->AddItemToStack(InventorySlateWidget);
}

void UPlayerMinerUIComponent::OnToggleCharacterMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerMinerInventoryUI::OnToggleMenu"));
	
	WidgetStack->AddItemToStack(CharacterMenuSlateWidget);
}

void UPlayerMinerUIComponent::OnCancelAction()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerMinerInventoryUI::OnCancelAction"));
	WidgetStack->RemoveLastItemFromStack();
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
	WidgetStack.Reset();
	InventorySlateWidget.Reset();
	CharacterMenuSlateWidget.Reset();
}

void UPlayerMinerUIComponent::InitializeUI()
{
	if(GEngine && GEngine->GameViewport)
	{
		WidgetStack = SNew(SDigumWidgetStack);
		WidgetStack->SetCanTick(true);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(_Container, SWeakWidget).PossiblyNullContent(WidgetStack.ToSharedRef()));
		
		if(OwningController.IsValid())
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(WidgetStack);
			OwningController->SetInputMode(InputMode);
			OwningController->bShowMouseCursor = true;
		}

		// Initialize Stuff
		InitializeInventoryWidget();
		InitializeCharacterMenuWidget();
	}

	if(OwningMiner.IsValid())
	{
		OwningMiner->OnToggleInventoryDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleInventory);
		OwningMiner->OnToggleCharacterMenuDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleCharacterMenu);
		OwningMiner->OnCancelActionDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnCancelAction);
	}
}

void UPlayerMinerUIComponent::InitializeInventoryWidget()
{
	// Initialize Inventory
	InventorySlateWidget =
		SNew(SDigumInventoryWindow)
		.InventoryComponent(OwningMiner->GetInventoryComponent())
		.HeightOverride(400)
		.WidthOverride(400) ;
}

void UPlayerMinerUIComponent::InitializeCharacterMenuWidget()
{
	CharacterMenuSlateWidget =
	SNew(SDigumWindow)
	.HeightOverride(400)
	.WidthOverride(400) ;
}
