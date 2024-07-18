// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DigumWorldPositioningComponent.h"
#include "GameFramework/Actor.h"
#include "Procedural/DigumWorldGenerator.h"
#include "Procedural/DigumWorldMap.h"
#include "DigumWorldMapActor.generated.h"

struct FDigumWorldRequestParams;
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

	UPROPERTY()
	bool bIsLocallyOwned = false;;

	UPROPERTY(Replicated)
	bool bInitializedMap = false;
	
public:

	DECLARE_MULTICAST_DELEGATE(FOnWorldLoaded);
	// Sets default values for this actor's properties
	ADigumWorldMapActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	
	FOnWorldLoaded OnWorldLoaded;
	
	UPROPERTY()
	TEnumAsByte<EDigumWorldMapPhase> CurrentPhase;
	virtual void BeginPlay() override;
	
	void OnInitializeSection();
	void OnSectionLoaded(FDigumWorldProceduralSection& DigumWorldProceduralSection);
	void OnAllSectionsLoaded();
	void OnWorldRequest(const FDigumWorldRequestParams& DigumWorldRequestParams);
	// Called when the game starts or when spawned
	
	virtual bool IsLocallyOwned();
	void OnRemoveSection(ADigumWorldActorSection* InDigumWorldActorSection);
	virtual void SpawnSection(FDigumWorldProceduralSection& InSection);

	ADigumWorldActorSection* GetSection(const int32 InX, const int32 InY);
	TArray<FDigumWorldProceduralSectionCoordinate> GetSectionCoordinatesInRect(const FDigumWorldProceduralSectionCoordinate& InStartCoordinate, const int32& HalfSize, const int32& XMin, const int32& XMax, const int32& YMin, const int32& YMax) const;

	int32 GetSectionIndex(const int32 InX, const int32 InY) const;

	void AddBlock_Internal(const FName& InBlockID, const FVector& InWorldLocation);
	void RemoveBlock_Internal(const FVector& InWoorldLocation);

	UFUNCTION(Server, Reliable)
	void Server_SpawnSection(const FDigumWorldProceduralSection& InSection);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnSection(const FDigumWorldProceduralSection& InSection);

	UFUNCTION(Server, Reliable)
	void Server_AddBlock(const FName& InBlockID, const FVector& InBlockLocation);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddBlock(const FName& InBlockID, const FVector& InBlockLocation);
	
	UFUNCTION(Server, Reliable)
	void Server_RemoveBlock(const FVector& InBlockLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemoveBlock(const FVector& InBlockLocation);

public:
	// void GenerateWorldMap();
	void SetPhase(const EDigumWorldMapPhase& InPhase) { CurrentPhase = InPhase; }
	EDigumWorldMapPhase GetPhase() const { return CurrentPhase; }

	void EnableSection(const int32 InX, const int32 InY);

	FOnWorldLoaded& GetOnWorldLoadedDelegate() { return OnWorldLoaded; }
	
	void TrySpawnSection(FDigumWorldProceduralSection& InSection);

	void TryExecuteAction(const FDigumWorldRequestParams& InParams);
	
	void BeginInitializeMap();
	
	APlayerController* GetOwningPlayerController() const { return OwningPlayerController; }
	
	void TryAddBlock(const FName& InBlockID, const FVector& InBlockLocation);
	void TryRemoveBlock(const FVector& InWorldLocation);

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Digum World Map Actor", CallInEditor, meta = (DisplayName = "Generate World Map Actor"))
	void Editor_GenerateWorldMap();
#endif
};
