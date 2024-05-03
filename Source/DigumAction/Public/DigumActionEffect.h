// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumActionEffect.generated.h"

UCLASS()
class DIGUMACTION_API ADigumActionEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumActionEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
