// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DummyPlayer.h"

#include "Actor/DigumWorldDynamicProceduralActor.h"
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
// Called when the game starts or when spawned
void ADummyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// SpawnMap();
}

