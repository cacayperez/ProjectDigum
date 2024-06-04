// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldPositioningComponent.generated.h"


struct FDigumWorldProceduralSectionCoordinate;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGUMWORLD_API UDigumWorldPositioningComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FVector GridSize;

	UPROPERTY()
	int32 SectionWidth;

	UPROPERTY()
	int32 SectionHeight;

	UPROPERTY()
	FVector2D UnitSectionSize;

	UPROPERTY()
	FDigumWorldProceduralSectionCoordinate CurrentCoordinate;

	UPROPERTY()
	FVector WorldOffset;

public:
	// Sets default values for this component's properties
	UDigumWorldPositioningComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
