// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DigumLocalTimerComponent.generated.h"

USTRUCT()
struct FDigumLocalTimer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FDateTime StartTime;

	UPROPERTY()
	float Duration = 1.0f;;
	
	UPROPERTY()
	float TimeScaleModifier = 1.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMCORE_API UDigumLocalTimerComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDigumLocalTimer> Timers;
	
public:
	// Sets default values for this component's properties
	UDigumLocalTimerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// void Add
};
