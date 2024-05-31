// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigumWorldActorSection.generated.h"

class ADigumWorldActorChild;
class UDigumWorldProceduralAsset;
struct FDigumWorldProceduralCoordinateArray;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorSection : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<ADigumWorldActorChild>> WorldChildActors;

public:
	// Sets default values for this actor's properties
	ADigumWorldActorSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeSection(FDigumWorldProceduralCoordinateArray* CoordinateArray, UDigumWorldProceduralAsset* ProceduralAsset);
	void DestroySection();
};