// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Player/Pawn/DigumGameMenuPlayerPawn.h"


// Sets default values
ADigumGameMenuPlayerPawn::ADigumGameMenuPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumGameMenuPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumGameMenuPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADigumGameMenuPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

