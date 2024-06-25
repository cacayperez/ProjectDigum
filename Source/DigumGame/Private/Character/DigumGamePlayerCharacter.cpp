// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/DigumGamePlayerCharacter.h"

#include "Components/DigumWorldPositioningComponent.h"
#include "GameMode/DigumGamePrimaryGameMode.h"
#include "Player/DigumMinerPlayerController.h"


ADigumGamePlayerCharacter::ADigumGamePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// PositioningComponent = CreateDefaultSubobject<UDigumWorldPositioningComponent>(TEXT("PositioningComponent"));
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
	// AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ADigumGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Character Begin Play"));
	// Register positioning component to gamemode

}

APlayerController* ADigumGamePlayerCharacter::GetPlayerController() const
{
	if(GetController())
	{
		return Cast<APlayerController>(GetController());
	}
	return nullptr;
}

float ADigumGamePlayerCharacter::GetVisibilityRadius() const
{
	return 3000.0f;
}


/*
void ADigumGamePlayerCharacter::InitializeInputBindings(ADigumMinerPlayerController* PlayerController)
{
	if(PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Initializing Input Bindings"));
		PlayerController->GetOnControllerInput_Move().AddDynamic(this, &ADigumMinerCharacter::Move);
		PlayerController->GetOnControllerInput_Jump().AddUObject(this, &ADigumGamePlayerCharacter::Jump);
		PlayerController->GetOnControllerInput_PrimaryAction().AddUObject(this, &ADigumMinerCharacter::PrimaryAction);
		PlayerController->GetOnControllerInput_SecondaryAction().AddUObject(this, &ADigumMinerCharacter::SecondaryAction);
		PlayerController->GetOnControllerInput_CancelAction().AddUObject(this, &ADigumMinerCharacter::CancelAction);
		PlayerController->GetOnControllerInput_ToggleInventory().AddUObject(this, &ADigumMinerCharacter::ToggleInventory);
		PlayerController->GetOnControllerInput_ToggleCharacterMenu().AddUObject(this, &ADigumMinerCharacter::ToggleCharacterMenu);
		PlayerController->GetOnControllerInput_SelectActionBar().AddUObject(this, &ADigumMinerCharacter::SelectActionBar);

		EnableInput(PlayerController);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to initialize bindings"));
	}
	
}

void ADigumGamePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("Character Possessed"));
	// InitializeInputBindings(NewController);
}*/

