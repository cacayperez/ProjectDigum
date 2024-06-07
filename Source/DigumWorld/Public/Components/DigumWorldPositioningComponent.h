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
	FDigumWorldProceduralSectionCoordinate PreviousCoordinate;
	
	UPROPERTY()
	FDigumWorldProceduralSectionCoordinate CurrentCoordinate;

	UPROPERTY()
	FVector WorldOffset;

public:
	// Sets default values for this component's properties
	UDigumWorldPositioningComponent();
	void CheckCoordinateChange();

protected:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnSectionCoordinateChanged, const AActor*, const FDigumWorldProceduralSectionCoordinate&, const FDigumWorldProceduralSectionCoordinate&);

	// Called when the game starts
	virtual void BeginPlay() override;

	FOnSectionCoordinateChanged OnSectionCoordinateChanged;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FDigumWorldProceduralSectionCoordinate GetCurrentCoordinate() const;

	FOnSectionCoordinateChanged& GetOnCoordinateChangedDelegate() { return OnSectionCoordinateChanged; }
};
