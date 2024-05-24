// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DigumPlayerController.h"


// Sets default values
ADigumPlayerController::ADigumPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
}

// Called every frame
void ADigumPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

