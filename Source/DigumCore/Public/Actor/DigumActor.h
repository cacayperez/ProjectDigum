// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumActor.generated.h"

UENUM(BlueprintType)
enum EDigumActorType : uint8
{
	DA_None,
	DA_WorldStatic,
	DA_WorldDynamic,
};

UCLASS()
class DIGUMCORE_API ADigumActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADigumActor(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY()
	TEnumAsByte<EDigumActorType> ActorType;
	
	virtual void SetActorType(EDigumActorType InActorType);
};
