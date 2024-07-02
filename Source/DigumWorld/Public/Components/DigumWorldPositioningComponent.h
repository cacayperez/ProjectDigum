// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldPositioningComponent.generated.h"


struct FDigumWorldProceduralSectionCoordinate;

USTRUCT()
struct FDigumWorldPositioningParams
{
	GENERATED_BODY()
public:
	FDigumWorldPositioningParams() {}
	FDigumWorldPositioningParams(const FUniqueNetIdRepl& InPlayerId, const FVector& InWorldLocation)
	{
		PlayerId = InPlayerId;
		WorldLocation = InWorldLocation;
	}
	FDigumWorldPositioningParams(const int32& InPlayerIndex, const FVector& InWorldLocation)
	{
		PlayerIndex = InPlayerIndex;
		WorldLocation = InWorldLocation;
	}

	UPROPERTY()
	FUniqueNetIdRepl PlayerId = FUniqueNetIdRepl();
	
	UPROPERTY()
	int32 PlayerIndex = -1;

	UPROPERTY()
	FVector WorldLocation = FVector::ZeroVector;
};

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

	UPROPERTY()
	bool bInitialized = false;

	UPROPERTY()
	FUniqueNetIdRepl PlayerId;
	
public:
	// Sets default values for this component's properties
	UDigumWorldPositioningComponent();
	void CheckCoordinateChange();

protected:
	// DECLARE_MULTICAST_DELEGATE_()
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSectionCoordinateChanged, const FDigumWorldPositioningParams&);

	// Called when the game starts
	virtual void BeginPlay() override;

	FOnSectionCoordinateChanged OnSectionCoordinateChanged;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FDigumWorldProceduralSectionCoordinate GetCurrentCoordinate() const;

	FOnSectionCoordinateChanged& GetOnCoordinateChangedDelegate() { return OnSectionCoordinateChanged; }
	void InitializePositioningComponent(const FVector& InGridSize, const int32& InSectionWidth, const int32& InSectionHeight, const FVector& InWorldOffset);
};
