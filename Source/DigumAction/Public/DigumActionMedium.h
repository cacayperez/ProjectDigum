// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumActionMedium.generated.h"

class UDigumAction;

UCLASS()
class DIGUMACTION_API ADigumActionMedium : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TSubclassOf<UDigumAction>> Actions;

	
public:
	// Sets default values for this actor's properties
	ADigumActionMedium();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
