// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/Miner/Components/PlayerMinerUIComponent.h"

#include "Character/Miner/DigumMinerCharacter.h"
#include "Character/Miner/Components/DigumGameInventoryComponent.h"
#include "Core/SDigumWidgetStack.h"
#include "Settings/DigumContentDefinition.h"
#include "Settings/DigumGameDeveloperSettings.h"
#include "UI/ActionBar/DigumActionBarWidget.h"
#include "UI/Inventory/DigumInventoryWidget.h"
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
	WidgetStack->AddItemToStack(InventoryWidget);
	
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
		
		GEngine->GameViewport->RemoveViewportWidgetContent(_Container.ToSharedRef());
	}

	// Clean up
	_Container.Reset();
	WidgetStack.Reset();
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
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	
			OwningController->SetInputMode(InputMode);
		
			OwningController->bShowMouseCursor = true;
		}

		// Initialize Widgets
		
		InitializeActionBarWidget();
		InitializeInventoryWidget();
		InitializeCharacterMenuWidget();
	}

	if(OwningMiner.IsValid())
	{
		OwningMiner->OnToggleInventoryDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleInventory);
		OwningMiner->OnToggleCharacterMenuDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnToggleCharacterMenu);
		OwningMiner->OnCancelActionDelegate().AddUObject(this, &UPlayerMinerUIComponent::OnCancelAction);
	}

	// OnToggleInventory();
}

void UPlayerMinerUIComponent::InitializeInventoryWidget()
{
	// TODO add default class to settings
	if(InventoryWidgetClass == nullptr)
	{
		const FDigumContentCategory* Content = UDigumGameDeveloperSettings::Get()->GetContentCategoryData(TEXT("Primary"));
		InventoryWidgetClass = Content->PlayerInventoryWidgetClass.LoadSynchronous();
	}

	if(InventoryWidgetClass)
	{
		InventoryWidget = UDigumWidget::Create<UDigumInventoryWidget>(this, InventoryWidgetClass);
	
		if(InventoryWidget)
		{
			// InventoryWidget->OnCreateWidget();
			InventoryWidget->SetInventoryComponent(OwningMiner->GetInventoryComponent());
		}
		
	}
}

void UPlayerMinerUIComponent::InitializeActionBarWidget()
{
	// TODO add default class to settings
	if(ActionBarWidgetClass == nullptr)
	{
		const FDigumContentCategory* Content = UDigumGameDeveloperSettings::Get()->GetContentCategoryData(TEXT("Primary"));
		ActionBarWidgetClass = Content->PlayerActionBarWidgetClass.LoadSynchronous();
	}

	if(ActionBarWidgetClass)
	{
		ActionBarWidget = UDigumWidget::Create<UDigumActionBarWidget>(this, ActionBarWidgetClass);
	
		if(ActionBarWidget)
		{
			// InventoryWidget->OnCreateWidget();
			ActionBarWidget->SetInventoryComponent(OwningMiner->GetInventoryComponent());
			WidgetStack->AddItemToStack(ActionBarWidget, EHorizontalAlignment::HAlign_Center, EVerticalAlignment::VAlign_Bottom);
		}
		
	}
}

void UPlayerMinerUIComponent::InitializeCharacterMenuWidget()
{
	CharacterMenuSlateWidget =
	SNew(SDigumWindow)
	.HeightOverride(400)
	.WidthOverride(400);
}
