// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/DigumMinerPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/DigumGamePlayerCharacter.h"
#include "Character/Miner/DigumMinerCharacter.h"
#include "Input/DigumInputSettingsAsset.h"
#include "Settings/DigumGameDeveloperSettings.h"


class UEnhancedInputLocalPlayerSubsystem;
class UDigumGameDeveloperSettings;
// Sets default values
ADigumMinerPlayerController::ADigumMinerPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADigumMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	const UDigumGameDeveloperSettings* DigumGameDeveloperSettings = GetDefault<UDigumGameDeveloperSettings>();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (DigumGameDeveloperSettings && Subsystem)
	{
		if(const UDigumInputSettingsAsset* InputSettings = DigumGameDeveloperSettings->PrimaryMinerInputSettings.LoadSynchronous())
		{
			if(InputSettings->MappingContext)
				Subsystem->AddMappingContext(InputSettings->MappingContext, 0);
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PrimaryMinerInputSettings Mapping Context is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Miner Input Settings is null!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add mapping context"));
	}

	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(DigumGameDeveloperSettings && EnhancedInputComponent)
	{
		if(const UDigumInputSettingsAsset* InputSettings = DigumGameDeveloperSettings->PrimaryMinerInputSettings.LoadSynchronous())
		{
			auto BindActionLambda = [this, EnhancedInputComponent](const FString& Context, UInputAction* Action, ETriggerEvent TriggerEvent, auto Func)
			{
				if(Action != nullptr)
				{
					UE_LOG(LogDigumMinerCharacter, Warning, TEXT("Binding Action %s"), *Context);
					EnhancedInputComponent->BindAction(Action, TriggerEvent, this, Func);
				}
				else
				{
					UE_LOG(LogDigumMinerCharacter, Error, TEXT("%s Action is null!"), *Context);
				}
			};
			BindActionLambda("Move", InputSettings->MoveAction, ETriggerEvent::Triggered, &ADigumMinerPlayerController::Move);
			BindActionLambda("Jump", InputSettings->JumpAction, ETriggerEvent::Started, &ADigumMinerPlayerController::Jump);
			BindActionLambda("Primary",InputSettings->PrimaryAction, ETriggerEvent::Started, &ADigumMinerPlayerController::PrimaryAction);
			BindActionLambda("Secondary",InputSettings->SecondaryAction, ETriggerEvent::Started,&ADigumMinerPlayerController::SecondaryAction);
			BindActionLambda("Cancel",InputSettings->CancelAction, ETriggerEvent::Started,&ADigumMinerPlayerController::CancelAction);
			BindActionLambda("Toggle Inventory", InputSettings->CharacterContextAction1, ETriggerEvent::Started, &ADigumMinerPlayerController::ToggleInventory);
			BindActionLambda("Toggle Character Menu", InputSettings->CharacterContextAction2, ETriggerEvent::Started, &ADigumMinerPlayerController::ToggleCharacterMenu);

			BindActionLambda("ActionBar 0", InputSettings->ActionBar_Action_0, ETriggerEvent::Started, &ADigumMinerPlayerController::SelectActionBar_0);
			BindActionLambda("ActionBar 1", InputSettings->ActionBar_Action_1, ETriggerEvent::Started, &ADigumMinerPlayerController::SelectActionBar_1);
			BindActionLambda("ActionBar 2", InputSettings->ActionBar_Action_2, ETriggerEvent::Started, &ADigumMinerPlayerController::SelectActionBar_2);
			BindActionLambda("ActionBar 3", InputSettings->ActionBar_Action_3, ETriggerEvent::Started, &ADigumMinerPlayerController::SelectActionBar_3);
			BindActionLambda("ActionBar 4", InputSettings->ActionBar_Action_4, ETriggerEvent::Started, &ADigumMinerPlayerController::SelectActionBar_4);
		}
	}
	else
	{
		UE_LOG(LogDigumMinerCharacter, Error, TEXT("Failed to bind actions"));
	}*/
}

/*
void ADigumMinerPlayerController::ClearInputBindings()
{
	OnControllerInput_Move.Clear();
	OnControllerInput_Jump.Clear();
	OnControllerInput_CancelAction.Clear();
	OnControllerInput_PrimaryAction.Clear();
	OnControllerInput_SecondaryAction.Clear();
	OnControllerInput_ToggleInventory.Clear();
	OnControllerInput_ToggleCharacterMenu.Clear();
	OnControllerInput_SelectActionBar.Clear();
}
*/

void ADigumMinerPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	/*InPawn->EnableInput(this);
	SetInputMode(FInputModeGameAndUI());*/

	// InitializeMinerController();
}/*
void ADigumMinerPlayerController::Move(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Move"));
	OnControllerInput_Move.Broadcast(InputActionValue);
}

void ADigumMinerPlayerController::Jump()
{
	OnControllerInput_Jump.Broadcast();
}

void ADigumMinerPlayerController::PrimaryAction()
{
	OnControllerInput_PrimaryAction.Broadcast();
}

void ADigumMinerPlayerController::SecondaryAction()
{
	OnControllerInput_SecondaryAction.Broadcast();
}

void ADigumMinerPlayerController::ToggleInventory()
{
	OnControllerInput_ToggleInventory.Broadcast();
}

void ADigumMinerPlayerController::ToggleCharacterMenu()
{
	OnControllerInput_ToggleCharacterMenu.Broadcast();
}

void ADigumMinerPlayerController::CancelAction()
{
	OnControllerInput_CancelAction.Broadcast();
}

void ADigumMinerPlayerController::SelectActionBar_0()
{
	OnControllerInput_SelectActionBar.Broadcast(0);
}

void ADigumMinerPlayerController::SelectActionBar_1()
{
	OnControllerInput_SelectActionBar.Broadcast(1);
}

void ADigumMinerPlayerController::SelectActionBar_2()
{
	OnControllerInput_SelectActionBar.Broadcast(2);
}

void ADigumMinerPlayerController::SelectActionBar_3()
{
	OnControllerInput_SelectActionBar.Broadcast(3);
}

void ADigumMinerPlayerController::SelectActionBar_4()
{
	OnControllerInput_SelectActionBar.Broadcast(4);
}

void ADigumMinerPlayerController::InitializeMinerController()
{
	if(GetPawn() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid pawn"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Controller : On Posses"));
	if(ADigumGamePlayerCharacter* Miner = Cast<ADigumGamePlayerCharacter>(GetPawn()))
	{
		ClearInputBindings();
		Miner->InitializeInputBindings(this);
	}
}

// Called every frame
void ADigumMinerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
*/

