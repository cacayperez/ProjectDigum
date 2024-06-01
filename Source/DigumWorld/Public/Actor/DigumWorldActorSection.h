// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldActorSection.generated.h"

struct FDigumWorldProceduralSection;
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

	UPROPERTY()
	FDigumWorldProceduralSection SectionData;

public:
	// Sets default values for this actor's properties
	ADigumWorldActorSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeSection(FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset);
	void DestroySection();
};