// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DigumActor.h"


// Sets default values
ADigumActor::ADigumActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADigumActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

