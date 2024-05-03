// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/DigumMinerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/DigumInputSettingsAsset.h"
#include "Settings/DigumGameDeveloperSettings.h"

DEFINE_LOG_CATEGORY(LogDigumMinerCharacter);

// Sets default values
ADigumMinerCharacter::ADigumMinerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ADigumMinerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		const UDigumGameDeveloperSettings* DigumGameDeveloperSettings = GetDefault<UDigumGameDeveloperSettings>();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (DigumGameDeveloperSettings && Subsystem)
		{
			if(const UDigumInputSettingsAsset* InputSettings = DigumGameDeveloperSettings->PrimaryMinerInputSettings.LoadSynchronous())
			{
				if(InputSettings->MappingContext)
					Subsystem->AddMappingContext(InputSettings->MappingContext, 0);
				else
				{
					UE_LOG(LogDigumMinerCharacter, Error, TEXT("PrimaryMinerInputSettings Mapping Context is null!"));
				}
			}
			else
			{
				UE_LOG(LogDigumMinerCharacter, Error, TEXT("Miner Input Settings is null!"));
			}
		}
		else
		{
			UE_LOG(LogDigumMinerCharacter, Error, TEXT("Failed to add mapping context"));
		}
	}
}

void ADigumMinerCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement
		// we only want to move on the x axis
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
	}
}

void ADigumMinerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	const UDigumGameDeveloperSettings* DigumGameDeveloperSettings = GetDefault<UDigumGameDeveloperSettings>();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(DigumGameDeveloperSettings && EnhancedInputComponent)
	{
		if(const UDigumInputSettingsAsset* InputSettings = DigumGameDeveloperSettings->PrimaryMinerInputSettings.LoadSynchronous())
		{
			auto BindActionLambda = [&](const FString& Context, UInputAction* Action, ETriggerEvent TriggerEvent, auto Func)
			{
				if(Action != nullptr)
					EnhancedInputComponent->BindAction(Action, TriggerEvent, this, Func);
				else
				{
					UE_LOG(LogDigumMinerCharacter, Error, TEXT("%s Action is null!"), *Context);
				}
			};

			BindActionLambda("Move", InputSettings->MoveAction, ETriggerEvent::Triggered, &ADigumMinerCharacter::Move);
			BindActionLambda("Jump", InputSettings->JumpAction, ETriggerEvent::Started, &ADigumMinerCharacter::Jump);
			BindActionLambda("Primary",InputSettings->PrimaryAction, ETriggerEvent::Started, &ADigumMinerCharacter::PrimaryAction);
			BindActionLambda("Secondary",InputSettings->SecondaryAction, ETriggerEvent::Started,&ADigumMinerCharacter::SecondaryAction);
			
		}
	}
	else
	{
		UE_LOG(LogDigumMinerCharacter, Error, TEXT("Failed to bind actions"));
	}
	
}

void ADigumMinerCharacter::Jump()
{
	Super::Jump();
}

void ADigumMinerCharacter::PrimaryAction()
{
}

void ADigumMinerCharacter::SecondaryAction()
{
}


