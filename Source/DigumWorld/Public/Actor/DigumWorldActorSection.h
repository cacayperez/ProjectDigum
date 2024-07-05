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

USTRUCT()
struct FDigumWorldChildActorsContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FName BlockID;

	UPROPERTY()
	ADigumWorldActorChild* ChildActor;
};

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


	DECLARE_MULTICAST_DELEGATE_OneParam(FDigumWorldSectionReadyForCleanup, ADigumWorldActorSection*);


public:
	// Sets default values for this actor's properties
	ADigumWorldActorSection();

	UPROPERTY(Replicated)
	FVector GridSize;

	UPROPERTY(Replicated)
	FVector2D SectionSize;
	

	
	FTimerHandle ReuseTimerHandle;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<FDigumWorldChildActorsContainer> ChildActorsContainers;

	UPROPERTY(Replicated, VisibleAnywhere)
	FDigumWorldProceduralSection SectionData;

	
	
	UPROPERTY()
	float CleanupTimer = 10.0f;

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
	void InitializeSpawnData(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection);
	void InitializeSection(const FVector2D& InSectionSize, FDigumWorldProceduralSection& InSection, UDigumWorldProceduralAsset* ProceduralAsset);
	void CreateChildActor(FDigumWorldProceduralCoordinateArray& InCoordinates);
	void AddBlock(const FName& InBlockID, const FVector& InLocation, const int32& WidthOffset = 0, const int32& HeightOffset = 0);
	// void CreateChildActor(UDigumWorldSwatchAsset* SwatchAsset);
	void DestroySection();
	void ResetSection();
	void EnableSection();
	void SetSectionEnabled(const bool& bValue);
	FDigumWorldProceduralSection GetSectionData() { return SectionData; }
	FDigumWorldSectionReadyForCleanup& GetDigumWorldSectionReadyForCleanupDelegate() { return OnSectionReadyForCleanup; }

	FDigumWorldChildActorsContainer* GetChildActorContainer(const FName& InBlockID);
	ADigumWorldActorChild* GetChildActor(const FName& InBlockID);

};