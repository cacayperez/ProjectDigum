// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Character/DigumGamePlayerCharacter.h"

#include "Components/DigumWorldPositioningComponent.h"
#include "GameMode/DigumGamePrimaryGameMode.h"


ADigumGamePlayerCharacter::ADigumGamePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PositioningComponent = CreateDefaultSubobject<UDigumWorldPositioningComponent>(TEXT("PositioningComponent"));
}

void ADigumGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Character Begin Play"));
	// Register positioning component to gamemode
	if(PositioningComponent)
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if(GameMode)
		{
			if(ADigumGamePrimaryGameMode* DigumGameMode = Cast<ADigumGamePrimaryGameMode>(GameMode))
			{
				UE_LOG(LogTemp, Warning, TEXT("Character Registering Positioning Component"));
				DigumGameMode->RegisterPositioningComponent(PositioningComponent);
			}
			
		}
	}
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

UDigumWorldPositioningComponent* ADigumGamePlayerCharacter::GetPositioningComponent() const
{
	return PositioningComponent;
}
