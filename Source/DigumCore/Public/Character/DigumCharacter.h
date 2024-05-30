// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "..\Interface\IDigumActorInterface.h"
#include "DigumCharacter.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (DigumCore))
class DIGUMCORE_API ADigumCharacter : public ACharacter, public IIDigumActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADigumCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual FVector GetForwardDirection() const override;
};
