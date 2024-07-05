// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "GameFramework/Actor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldMapActor.generated.h"

struct FDigumWorldProceduralRules;
struct FDigumWorldMap;
class UDigumWorldMapSectionComponent;
class ADigumWorldActorSection;

UENUM()
enum EDigumWorldMapPhase : uint8
{
	DigumWorldMapPhase_None,
	DigumWorldMapPhase_Initializing,
	DigumWorldMapPhase_Generating,
	DigumWorldMapPhase_Generated,
	DigumWorldMapPhase_Active
};

UCLASS()
class DIGUMWORLD_API ADigumWorldMapActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDigumWorldMapSectionComponent> SectionComponent;
	
	UPROPERTY(Replicated)
	FDigumWorldMap WorldMap;
	
	UPROPERTY(Replicated, EditAnywhere, Category = "Digum World Map", meta = (AllowPrivateAccess = "true"))
	FDigumWorldProceduralRules ProceduralRules;

	UPROPERTY(Replicated)
	TArray<ADigumWorldActorSection*> SectionActors;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY(Replicated)
	FVector WorldOffset;
	
public:

	DECLARE_MULTICAST_DELEGATE(FOnWorldLoaded);
	// Sets default values for this actor's properties
	ADigumWorldMapActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	
	FOnWorldLoaded OnWorldLoaded;
	
	UPROPERTY()
	TEnumAsByte<EDigumWorldMapPhase> CurrentPhase;
	
	void OnInitializeSection();
	void OnSectionLoaded(FDigumWorldProceduralSection& DigumWorldProceduralSection);
	void OnAllSectionsLoaded();	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnRemoveSection(ADigumWorldActorSection* InDigumWorldActorSection);
	virtual void SpawnSection(FDigumWorldProceduralSection& InSection);

	ADigumWorldActorSection* GetSection(const int32 InX, const int32 InY);
	TArray<FDigumWorldProceduralSectionCoordinate> GetSectionCoordinatesInRect(const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin, const int32& XMax, const int32& YMin, const int32& YMax) const;

	int32 GetSectionIndex(const int32 InX, const int32 InY) const;
public:
	// void GenerateWorldMap();
	void SetPhase(const EDigumWorldMapPhase& InPhase) { CurrentPhase = InPhase; }
	EDigumWorldMapPhase GetPhase() const { return CurrentPhase; }

	void EnableSection(const int32 InX, const int32 InY);

	FOnWorldLoaded& GetOnWorldLoadedDelegate() { return OnWorldLoaded; }

	UFUNCTION(Server, Reliable)
	void Server_SpawnSection(const FDigumWorldProceduralSection& InSection);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnSection(const FDigumWorldProceduralSection& InSection);
	
	void TrySpawnSection(FDigumWorldProceduralSection& InSection);

	void BeginInitializeMap();
	
	void SetOwningPlayerController(APlayerController* NewPlayer)
	{
		OwningPlayerController = NewPlayer;
		SetOwner(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("ADigumWorldMapActor::SetOwningPlayerController: %s"), *NewPlayer->GetName());

	}
	APlayerController* GetOwningPlayerController() const { return OwningPlayerController; }
	void AddBlock(const FName& InBlockID, const FVector& InBlockLocation);
};
