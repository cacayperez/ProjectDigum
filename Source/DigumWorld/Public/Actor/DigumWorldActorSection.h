// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "DigumWorldActorSection.generated.h"

struct FDigumWorldProceduralSection;
class UDigumVisibilityComponent;
class ADigumWorldActorChild;
class UDigumWorldProceduralAsset;
class UBoxComponent;
struct FDigumWorldProceduralCoordinateArray;

UCLASS()
class DIGUMWORLD_API ADigumWorldActorSection : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Digum World Actor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	/*UPROPERTY()
	TObjectPtr<UBoxComponent> BoxComponent;*/
	
	UPROPERTY()
	TObjectPtr<UDigumVisibilityComponent> VisibilityComponent;
	/*UPROPERTY()
	TArray<TWeakObjectPtr<ADigumWorldActorChild>> WorldChildActors;*/

	UPROPERTY()
	TMap<FName, ADigumWorldActorChild*> WorldChildActors;

	UPROPERTY(VisibleAnywhere)
	FDigumWorldProceduralSection SectionData;

	UPROPERTY()
	FVector GridSize;

	UPROPERTY()
	FVector2D SectionSize;
	
	UPROPERTY()
	int32 SectionX;

	UPROPERTY()
	int32 SectionY;

	DECLARE_MULTICAST_DELEGATE_OneParam(FDigumWorldSectionReadyForCleanup, ADigumWorldActorSection*);


public:
	// Sets default values for this actor's properties
	ADigumWorldActorSection();

	FTimerHandle ReuseTimerHandle;

protected:
	UPROPERTY()
	float CleanupTimer = 5.0f;

	UPROPERTY()
	FTimerHandle CleanupTimerHandle;
	
	void OnSetWorldVisibility(bool bValue);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void CleanupSection();

	FDigumWorldSectionReadyForCleanup OnSectionReadyForCleanup;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Reinitialize();
	void InitializeSection(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset);
	void CreateChildActor(FDigumWorldProceduralCoordinateArray& InCoordinates);
	void AddBlock(const FName& InBlockID, const FVector& InLocation, const int32& WidthOffset = 0, const int32& HeightOffset = 0);
	// void CreateChildActor(UDigumWorldSwatchAsset* SwatchAsset);
	void DestroySection();
	void ResetSection();
	void EnableSection();
	FDigumWorldProceduralSection GetSectionData() { return SectionData; }
	FDigumWorldSectionReadyForCleanup& GetDigumWorldSectionReadyForCleanupDelegate() { return OnSectionReadyForCleanup; }
	/*int32 GetX() const;
	int32 GetY() const;*/
	// void SetBoxExtent(const FVector& InExtent) const;
	// bool IsFlaggedForReuse() const { return bFlaggedForReuse; }
};