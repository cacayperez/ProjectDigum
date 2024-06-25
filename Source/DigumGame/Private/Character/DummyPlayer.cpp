// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DummyPlayer.h"

#include "Asset/DigumAssetManager.h"
#include "Camera/CameraComponent.h"
#include "Character/DigumGamePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ADummyPlayer::ADummyPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Root);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
}

void ADummyPlayer::SpawnPlayerCharacter()
{
	/*if(GetController() && PlayerCharacterClass)
	{
		ADigumMinerCharacter* Character = GetWorld()->SpawnActor<ADigumMinerCharacter>(PlayerCharacterClass, GetActorLocation(), GetActorRotation());
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if(Character)
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
			PlayerController->Possess(Character);
			
			
			// Character->InitializeInputBindings();
			// Destroy();
		}
	}*/
}

// Called when the game starts or when spawned
void ADummyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacterClass = UDigumAssetManager::GetSubclass(SoftPlayerCharacterClass);
	if(PlayerCharacterClass)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADummyPlayer::SpawnPlayerCharacter, SpawnDelay, false);
	}
}
