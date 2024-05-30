// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DigumCharacter.h"

#include "Movement/DigumMovementComponent.h"

ADigumCharacter::ADigumCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass(CharacterMovementComponentName, UDigumMovementComponent::StaticClass()))
{
}

// Called when the game starts or when spawned
void ADigumCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADigumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADigumCharacter::GetForwardDirection() const
{
	return GetActorForwardVector();
}
